#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include "php.h"
#include "php_ini.h"
#include "ext/standard/info.h"
#include "php_afk.h"

#define HTTP_CONTRONLLER_PARAM "c"
#define HTTP_ACTION_PARAM "a"

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
}

zend_function_entry afk_app_method[] = {
	ZEND_ME(afk_app,    run,  afk_app_run_arginfo,   ZEND_ACC_PUBLIC)
	{NULL, NULL, NULL}
};


