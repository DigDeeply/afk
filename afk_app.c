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
	php_printf("%s", controller_path);
	if( (fp = fopen(controller_path, "r")) != NULL){
		fclose(fp);
		int dummy = 1;
		controller_path = "/home/s/www/fukun/clang/php-5.2.6/ext/afk/app/controller/index.php";
		zend_hash_add(&EG(included_files), controller_path, strlen(controller_path)+1, (void *)&dummy, sizeof(int), NULL);
	}else{
		char *error;
		spprintf(&error, 0, "cann't find file %s", controller_path);
		zend_error(1, error);
	}
	RETURN_BOOL(1);
}

zend_function_entry afk_app_method[] = {
	ZEND_ME(afk_app,    run,  afk_app_run_arginfo,   ZEND_ACC_PUBLIC)
	{NULL, NULL, NULL}
};


