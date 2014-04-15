#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include "php.h"
#include "php_ini.h"
#include "ext/standard/info.h"
#include "php_afk.h"

#define HTTP_CONTRONLLER_PARAM "c"
#define HTTP_ACTION_PARAM "a"
#define APP_DIR "app"

zend_class_entry *afk_app_ce;

ZEND_BEGIN_ARG_INFO_EX(afk_app_run_arginfo, 0, 0, 0)
ZEND_END_ARG_INFO()

ZEND_METHOD(afk_app, run){
	zval **uri;
	char *c=NULL, *a=NULL;
	zval *arr = PG(http_globals)[TRACK_VARS_GET];
	//从http_globals中寻找get参数，确认Controller和action的值。
	if(arr && Z_TYPE_P(arr) == IS_ARRAY){
		if(zend_hash_find(HASH_OF(arr), HTTP_CONTRONLLER_PARAM, strlen(HTTP_CONTRONLLER_PARAM)+1, (void **)&uri) == SUCCESS){
			c = Z_STRVAL_PP(uri);
		}else{
			c = "index";
		}
		if(zend_hash_find(HASH_OF(arr), HTTP_ACTION_PARAM, strlen(HTTP_ACTION_PARAM)+1, (void **)&uri) == SUCCESS){
			a = Z_STRVAL_PP(uri);
		}else{
			a = "index";
		}
	}
	//寻找对应的Controller和action方法所在的文件。
	char *controller_path;
	spprintf(&controller_path, 0, "%s/controller/%s.php", APP_DIR, c);

	FILE *fp;
	php_printf("%s\n", controller_path);
	//文件存在则引入该文件。不存在则报错。
	if( (fp = fopen(controller_path, "r")) != NULL){
		fclose(fp);
		int dummy = 1;

		zend_file_handle file_handle;
		zend_op_array *op_array;

		file_handle.filename = controller_path;
		file_handle.free_filename = 0;
		file_handle.type = ZEND_HANDLE_FILENAME;
		file_handle.opened_path = NULL;
		file_handle.handle.fp = NULL;

		op_array = zend_compile_file(&file_handle, ZEND_INCLUDE TSRMLS_CC);
		if (op_array && file_handle.handle.stream.handle) {
			int dummy = 1;         

			if (!file_handle.opened_path) {
				file_handle.opened_path = controller_path;
			}

			php_printf("opened_path: %s\n", file_handle.opened_path);
			zend_hash_add(&EG(included_files), file_handle.opened_path, strlen(file_handle.opened_path)+1, (void *)&dummy, sizeof(int), NULL);
		}
		zend_destroy_file_handle(&file_handle TSRMLS_CC);
		if(op_array){
			php_printf("execute op_array \n");

			//保存旧的环境变量
			zval ** __old_return_value_pp   = EG(return_value_ptr_ptr); 
			zend_op ** __old_opline_ptr     = EG(opline_ptr); 
			zend_op_array * __old_op_array  = EG(active_op_array);
			zend_function_state * __old_func_state = EG(function_state_ptr); 

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
		spprintf(&error, 0, "cann't find file %s", controller_path);
		zend_error(1, error);
	}
	//dispatcher 调用分发的请求。
	/**
	 *在EG(class_table)查找相应的类，然后调用它的方法。
	 */
	zend_class_entry **class = NULL;
	char  *class_name = emalloc(strlen(c)+strlen("Controller")+1);
	class_name = strcpy(class_name, c);
	class_name = strcat(class_name, zend_str_tolower_dup("Controller", strlen("Controller")+1)); //Notice: class name need tolower.
	if(zend_hash_find(EG(class_table), class_name, strlen(class_name)+1, (void *)&class) != SUCCESS){
		char *error;
		spprintf(&error, 0, "cann't find the controller class: %s ", class_name);
		php_printf("%s", class_name);
		efree(class_name);
		efree(class);
		zend_error(1, error);
	}
	efree(class_name);

	zval *obj, *function_name;
	MAKE_STD_ZVAL(obj);
	MAKE_STD_ZVAL(function_name);
	object_init_ex(obj, *class);
	//php_var_dump(&obj, 1 TSRMLS_CC);

	ZVAL_STRINGL(function_name, "indexaction", strlen("indexaction"), 1);
	call_user_function(&(*class)->function_table, &obj, function_name, NULL, 0, NULL TSRMLS_CC);

	zval_ptr_dtor(&obj);
	zval_ptr_dtor(&function_name);
	efree(class);

	RETURN_BOOL(1);
}

zend_function_entry afk_app_method[] = {
	ZEND_ME(afk_app,    run,  afk_app_run_arginfo,   ZEND_ACC_PUBLIC)
	{NULL, NULL, NULL}
};


