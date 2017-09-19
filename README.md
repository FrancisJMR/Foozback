# Foozback
## A single-page PHP AJAX framework

Foozback was a project started in 2007 to create simple AJAX one-page websites in PHP with automatic DOM id's and other neat features using prototype.js. The goal was to keep code as short as possible while minimizing the number of refreshs from a user interface perspective.

## Example

A simple yet full image thumbnail listing page with admin login to add/delete images to the media folder in less than 50 lines of code.

```
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

require_once('footer.php');
?>
```