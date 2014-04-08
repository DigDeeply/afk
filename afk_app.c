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
		destroy_op_array(op_array TSRMLS_CC);
		efree(op_array);
	}else{
		char *error;
		spprintf(&error, 0, "cann't find file %s", controller_path);
		zend_error(1, error);
	}
	//dispatcher 调用分发的请求。
	/**
	 *在EG(class_table)查找相应的类，然后调用它的方法。
	 */

	RETURN_BOOL(1);
}

zend_function_entry afk_app_method[] = {
	ZEND_ME(afk_app,    run,  afk_app_run_arginfo,   ZEND_ACC_PUBLIC)
	{NULL, NULL, NULL}
};


