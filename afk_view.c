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


	RETURN_BOOL(1);
}/*}}}*/

zend_function_entry afk_view_method[] = {
	ZEND_ME(afk_view,    display,  afk_view_display_arginfo,   ZEND_ACC_PUBLIC)
	{NULL, NULL, NULL}
};


