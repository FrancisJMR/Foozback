<?php

$path = substr(dirname(__FILE__), 0, strrpos(dirname(__FILE__), '/'));
ini_set("include_path", ".:$path:$path/core/:$path/extensions/:$path/phpThumb/");

require_once('objects.inc');
require_once('helpers.inc');
require_once('applications.inc');

if ($_SESSION['reqlogin']) {
	$login_app = new login();	
	if (!$login_app->validateLogin()) {
		echo 'You are not loged in.';
		exit;
	}
}

//$authenticate->getSession
$posts = $_POST;

$cb_action = $posts[FORM_ACTION];

if ($cb_action === ACTION_DELETE) {
	$file_name = $path.'/'.$posts['file_name'];
	if (isset($file_name)) $result = unlink($file_name);
	echo 'deleted '.$file_name;
}

if ($cb_action === ACTION_SAVE) {
	// serialize our data and write it to a file.
	$data = serialize($posts);
	$fname = 'section_form_data.dat';
	if (!$fp = fopen($path.'/'.$fname, 'a+t')) {
		echo 'Cannot Save Form, due to resource Error.';
	} elseif (FALSE === fputs($fp, $data)) {
		echo 'Cannot Save Form, due to file Error.';
	} else {
		echo 'Form Saved Successfully!<br>';
		$action_download_file = new action(ACTION_GOTO, $fname);
		$action_download_file->setLabel($fname);
		echo $action_download_file->getOutput();
		fclose($fp);
	}
}

if ($cb_action === ACTION_PANE) {
	print_r($posts);
	/*
	
	foreach($posts as $key => $val) {
		$output = '';
		if ($key != FORM_ACTION) {
			$obj = new ($val)('index.php');
			$obj->prepare();
			$output .= $obj->getOutput();
		}
	}
	echo $output;
	*/
}

// Handle Login
if ($cb_action === 'login') {
	// serialize our data and write it to a file.
	$login_app = new login();
	if ($login_app->validateLogin($posts['user'], $posts['pass'])) {
		echo 'You have loged in.';
		
		//header('Location: '.$_SERVER['REQUEST_URI']);
		$file_upload = new fileUpload('root');
		$file_upload->fileUploadForm('art_upload', 'index.php', 3);
		
	} else {
		echo 'Login Failed.';
	}
}
?>