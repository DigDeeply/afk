#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include "php.h"
#include "php_ini.h"
#include "ext/standard/info.h"
#include "php_afk.h"

zend_class_entry *afk_view_ce;

ZEND_BEGIN_ARG_INFO_EX(afk_view_display_arginfo, 0, 0, 1)
	ZEND_ARG_INFO(0, tpl)
	ZEND_ARG_INFO(0, vars)
ZEND_END_ARG_INFO()

ZEND_METHOD(afk_view, display){/*{{{*/

	char *tpl;
	int tpl_len;
	zval *vars;

	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "s|a", &tpl, &tpl_len, &vars) == FAILURE) {
		return;
	}

	php_printf("%s\n", tpl);
	php_var_dump(&vars, 1 TSRMLS_CC);

	//寻找view所在的文件。
	char *view_path;
	spprintf(&view_path, 0, "%s/view/%s.php", APP_DIR, tpl);

	FILE *fp;
	php_printf("%s\n", view_path);
	//文件存在则引入该文件。不存在则报错。
	if( (fp = fopen(view_path, "r")) != NULL){
		fclose(fp);
		int dummy = 1;

		zend_file_handle file_handle;
		zend_op_array *op_array;

		file_handle.filename = view_path;
		file_handle.free_filename = 0;
		file_handle.type = ZEND_HANDLE_FILENAME;
		file_handle.opened_path = NULL;
		file_handle.handle.fp = NULL;

		op_array = zend_compile_file(&file_handle, ZEND_INCLUDE TSRMLS_CC);
		if (op_array && file_handle.handle.stream.handle) {
			int dummy = 1;         

			if (!file_handle.opened_path) {
				file_handle.opened_path = view_path;
			}

			php_printf("opened_path: %s\n", file_handle.opened_path);
			zend_hash_add(&EG(included_files), file_handle.opened_path, strlen(file_handle.opened_path)+1, (void *)&dummy, sizeof(int), NULL);
		}
		zend_destroy_file_handle(&file_handle TSRMLS_CC);
		if(op_array){
			//php_printf("execute op_array \n");

			//保存旧的环境变量
			zval ** __old_return_value_pp   = EG(return_value_ptr_ptr); 
			zend_op ** __old_opline_ptr     = EG(opline_ptr); 
			zend_op_array * __old_op_array  = EG(active_op_array);
			zend_function_state * __old_func_state = EG(function_state_ptr); 

			//把模版变量的值放到当前作用域
			if (vars && Z_TYPE_P(vars) == IS_ARRAY) {
				zval **entry;
				char *var_name;
				ulong num_key;
				uint var_name_len;
				HashPosition pos;
				for(zend_hash_internal_pointer_reset_ex(Z_ARRVAL_P(vars), &pos);
					zend_hash_get_current_data_ex(Z_ARRVAL_P(vars), (void **)&entry, &pos) == SUCCESS;
					zend_hash_move_forward_ex(Z_ARRVAL_P(vars), &pos)) {
					if (zend_hash_get_current_key_ex(Z_ARRVAL_P(vars), &var_name, &var_name_len, &num_key, 0, &pos) != HASH_KEY_IS_STRING) {
						continue;
					}

					/* GLOBALS protection */
					if (var_name_len == sizeof("GLOBALS") && !strcmp(var_name, "GLOBALS")) {
						continue;
					}

					if (var_name_len == sizeof("this")  && !strcmp(var_name, "this") && EG(scope) && EG(scope)->name_length != 0) {
						continue;
					}

					ZEND_SET_SYMBOL_WITH_LENGTH(EG(active_symbol_table), var_name, var_name_len,
							*entry, ZVAL_REFCOUNT(*entry) + 1, 0 /**PZVAL_IS_REF(*entry)*/);
				}
			}

			//执行op_array
			zval *result = NULL;
			EG(return_value_ptr_ptr) = &result;
			EG(active_op_array) = op_array;
			zend_execute(op_array TSRMLS_CC);
			destroy_op_array(op_array TSRMLS_CC);
			efree(op_array);

			//恢复旧的环境变量
			EG(return_value_ptr_ptr) = __old_return_value_pp;
			EG(opline_ptr)           = __old_opline_ptr; 
			EG(active_op_array)      = __old_op_array; 
			EG(function_state_ptr)   = __old_func_state;
		}
	}else{
		char *error;
		spprintf(&error, 0, "cann't find file %s", view_path);
		zend_error(1, error);
	}

	RETURN_BOOL(1);
}/*}}}*/

zend_function_entry afk_view_method[] = {
	ZEND_ME(afk_view,    display,  afk_view_display_arginfo,   ZEND_ACC_PUBLIC)
	{NULL, NULL, NULL}
};


