<?php

class indexController{

	public function indexAction(){
		$msg = "\nIndexController->indexAction\n";
		if(PHP_SAPI == "cli"){
			echo $msg;
		}else{
			echo nl2br($msg);
		}
		return false;
	}

	public function testAction(){
		$msg = "\nIndexController->testAction\n";
		if(PHP_SAPI == "cli"){
			echo $msg;
		}else{
			echo nl2br($msg);
		}
	}
}
