<?php
$br = (php_sapi_name() == "cli")? "":"<br>";

if(!extension_loaded('afk')) {
	dl('afk.' . PHP_SHLIB_SUFFIX);
}
$module = 'afk';
$functions = get_extension_funcs($module);
echo "Functions available in the test extension:$br\n";
foreach($functions as $func) {
    echo $func."$br\n";
}
echo "$br\n";
$function = 'confirm_' . $module . '_compiled';
if (extension_loaded($module)) {
	$str = $function($module);
} else {
	$str = "Module $module is not compiled into PHP";
}
echo "$str\n";

//self test.
$app = new afk_app();
$app->run();

$view = new afk_view();
$view->display('index', array('a'=>'b', 'b'=>'hello', 'c'=>'world'));

//include("app/controller/index.php");
//$c = new indexController();
//$c->indexAction();
//$c->testAction();
?>
