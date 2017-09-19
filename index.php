<?php


$BASE_PATH = dirname(__FILE__);
$DEPENDS_PATH  = ".;".$BASE_PATH;
$DEPENDS_PATH .= ":".$BASE_PATH."/core";
$DEPENDS_PATH .= ":".$BASE_PATH."/extensions";

$DEPENDS_PATH .= ":".$BASE_PATH."/phpThumb";

ini_set("include_path", ini_get("include_path").":".$DEPENDS_PATH);

require_once('header.php');
require_once('helpers.inc');
require_once('config.inc');
require_once('models.inc');
require_once('substrate.inc');
require_once('objects.inc');
require_once('applications.inc');
require_once('panes.inc');

// Import website callbacks
//require_once('starkers_form.php');

echo '<h1>My Site</h1>';
echo '<h2>Subtitle</h2>';

echo 'Introduction text';

// Root Files
$file_list = new fileList('images', 'media');
//$file_list->setTitle('Serie 1');
//$file_list->displayIndex(true);
$file_list->setDelete(true);
//$file_list->displaySize(true);
$file_list->displaySubDirs(true);
$file_list->displayThumbs(true);
$file_list->setThumbSize('400px', null);
echo $file_list->getOutput();

$file_upload = new fileUpload('media');
$file_upload->fileUploadForm('upload', 'index.php', 3);
$file_upload->uploadFile();


$log_app = new login();
$log_app->setDiv('login');
$log_app->buildLoginForm();
$log_app->validateLogin();
echo $log_app->getOutput();


// Stuff files
/*
$file_list = new fileList('stuff_list', 'root');
$file_list->setTitle('My title');
//$file_list->displayIndex(true);
$file_list->displayThumbs(true);
$file_list->setThumbSize('60px', '48px');
echo $file_list->getOutput();
*/

// FORM EXAMPLE
/*
$form = new form('index.php', 'post', 'some_form');

$base_fc = new radioField('object_type', 'base_object', 'object_type', 'My object');
$base_fc->setChecked();
$base_lc = new radioField('thing_type', 'base_thing', 'thing_type', 'My thing');

$form->addField($base_fc, 'Step 1: Choose an object');
$form->addField($base_lc, 'Step 2: Choose a thing');

$form->setCallback('callback_form.php', ACTION_CALC_BASE);

$form->setTitle('Step 1');

echo $form->getOutput();
*/


// APPLICATION EXAMPLES
//$log_app = new login();
//$log_app->buildLoginForm();
//echo $log_app->getOutput();


// MENU and PANE EXAMPLE
/*
$menu_app  = new menu();

//$sec_form_app = new sectionForm('index.php');
//$sec_form_app->prepare();
//$form_pane = new pane('Form');
//$form_pane->addApplication($sec_form_app);
//$menu_app->addPane($form_pane);
//$menu_app->setMenuDefaultPane($form_pane);

$log_app = new login();
$log_app->buildLoginForm();
$login_pane = new pane('Login');
$login_pane->addApplication($log_app);
$menu_app->addPane($login_pane);

$del_file_list = new fileDeleteList('Demo File List', 'root');
$files_pane = new pane('Files');
$files_pane->addApplication($del_file_list);
$menu_app->addPane($files_pane);

//echo $sec_form_app->getOutput();
echo $menu_app->getOutput();
*/

require_once('footer.php');
?>