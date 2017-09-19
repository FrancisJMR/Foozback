<?php

require_once('models.inc');
require_once('substrate.inc');
require_once('helpers.inc');

// phpThumb includes
require_once('phpThumb.config.php');
require_once('phpthumb.functions.php');
//require_once('phpThumb.php');

// NAVIGATION CLASSES

// form params
define('FORM_ACTION', 'action');

// action types
define('ACTION_GOTO'  , 'goto');
define('ACTION_DELETE', 'delete');
define('ACTION_EDIT'  , 'edit');
define('ACTION_SAVE'  , 'save');
define('ACTION_PANE'  , 'pane');

define('ACTION_TRANSFORM', 'transform');
define('ACTION_FILE_UPLOAD', 'file_upload');
class action extends substrate {

	public $action_type;
	public $target;
	public $attributes_ar;
	public $image_path;
	public $script;
	public $label;

	function __construct($action_type, $dest=null) {
		parent::__construct();
		
		$this->action_type    = $action_type;
		$this->dest           = $dest;
		$this->attributes_ar  = array();
		$this->directories_ar = array();
		$this->img_attribs_ar  = array();
	}

	function setAttribute($name, $value) {
		$this->attributes_ar[$name] = $value;
	}

	function addDirectory($name) {
		$this->directories_ar[] = $name;
	}

	function setDestination($dest){
		$this->dest = $dest;
	}

	function setLabel($label){
		$this->label = $label;
	}

	function setImage($image_path) {
		$this->image_path = $image_path;
	}

	function setImageAttribute($name, $value) {
		$this->img_attribs_ar[$name] = $value;
	}
	
	function setScript($event_name, $script) {
		$this->script = $event_name.'="'.$script.'"';
	}

	function getOutput() {

		$action_type = $this->action_type;
		
		$atrb = '';
		foreach($this->attributes_ar as $name => $value) {
			$atrb .= $name.' = "'.$value.'" ';
		}

		$script = isset($this->script) ? $this->script : '';

		$dest = '#';
		if(isset($this->dest)) $dest = $this->dest;

		$action_str .= '<a href="'.$dest.'" '.$atrb.' '.$script.'>';
		if (isset($this->image_path)) {
			$img_atrb = '';
			foreach($this->img_attribs_ar as $name => $value) {
				$img_atrb .= $name.' = "'.$value.'" ';
			}
			//$action_str .= '<img src="'.$this->image_path.'" '.$img_atrb.'>';
			$action_str .= '<img src="phpThumb/phpThumb.php/300;/'.$this->image_path.'" >';
		} elseif (isset($this->label)) {
			$action_str .= $this->label;
		} else {
			$action_str .= $dest;
		}
		$action_str .= '</a>';

		return $action_str;
	}
}


class form extends substrate {
	
	public $action;
	public $method;
	public $id;
	
	public $form_fields_ar;
	private $sections_ar;
	
	private $callback_filename;
	private $callback_action;
	
	private $form_title;
	private $form_comments;
	
	function __construct($action, $method, $id) {
		//set defaults and initialize
		$this->action = $action;
		$this->method = $method;
		$this->id     = $id;

		$this->form_fields_ar = array();
		$this->show_fields_ar = array();
		
		$this->sections_ar = array();
		
		$this->callback_filename = 'core/objects_cb.php';
		$this->callback_action = ACTION_SAVE;
	}

	function setAction($action) {
		$this->action = $action;
	}

	function setMethod($method) {
		$this->method = $method;
	}

	function setId($id) {
		$this->id = $id;
	}

	function setTitle($title){
		$this->form_title = $title;
	}
	
	function setComments($comments) {
		$this->form_comments = $comments;
	}

	function setCallback($callback_filename, $callback_action){
		$this->callback_filename = $callback_filename;
		$this->callback_action   = $callback_action;
	}

	function getId() {
		return $this->id;
	}
	
	function getFormField($ff_name) {
		if (isset($this->form_fields_ar[$ff_name])) return $this->form_fields_ar[$ff_name];
	}

	/**
	* All fields of a form are in at least the default section,
	* but more than one section can be created for multi-pane forms.
	*/
	function addField($field, $section_name=null) {

		if (isset($section_name)) { 
			$section_id = $section_name.'_'.$this->id;
		} else {
			$section_id = 'section_'.$this->id;
		}

		if (isset($this->sections_ar[$section_name])) {
			$this->sections_ar[$section_name]->addField($field);
		} else {
			$this->sections_ar[$section_name] = new section($section_name, $section_id, $this->id);
			$this->sections_ar[$section_name]->addField($field);
		}
	}

	function getOutput(){
		
		$form_id = $this->id;
		$callback_filename = $this->callback_filename;
		
		$form = '<FORM '; 
		$form .= 'id="'.$form_id.'" ';
		if (isset($this->action)) $form .= 'ACTION="'.$this->action.'" ';
		if (isset($this->method)) $form .= 'METHOD="'.$this->method.'" ';
		$form .= '>';
		
		//$form .= '<div>'.$this->form_title.'</div>';
		
		$section_count = count($this->sections_ar);
		$si = 0;
		//prepare section actions and get their output
		foreach($this->sections_ar as $section_name => $section) {
			
			$section_id = $section->getId();
			if ($si < $section_count-1) { 
				$label  = 'Next';
				$type   = 'button';
				$script = "form_".$section->parent_id.".nextSection('$section_id'); return false;";
				
				$button_field = new buttonField(SECTION_ACTION_NEXT, $label, $value, $type, $script);
				$section->addAction(SECTION_ACTION_NEXT, $button_field);
			}
			if ($si > 0) {
				$label  = 'Prev';
				$type   = 'button';
				$script = "form_".$section->parent_id.".prevSection('$section_id'); return false;";
				
				$button_field = new buttonField(SECTION_ACTION_PREV, $label, $value, $type, $script);
				$section->addAction(SECTION_ACTION_PREV, $button_field);
			}
			if ($si == $section_count-1) {
				
				if($this->callback_action == ACTION_SAVE) $label = 'Submit'; else $label = 'Next Step';
					
				$type   = 'button';
				
				$script = "form_".$section->parent_id.".saveForm(); return false;";
				$action_field = new inputField(FORM_ACTION, null, 'hidden', $this->callback_action );
				$section->addField($action_field);
				
				$button_field = new buttonField(SECTION_ACTION_SAVE, $label, $value, $type, $script);
				$section->addAction(SECTION_ACTION_SAVE, $button_field);
			}
			$form .= $section->getOutput(); //should be doing prepare()...right!
			$si++;
		}
		
		//$form .= '<p>'.$this->form_comments.'</p>';
		
		$form .= '</form>';
		$form .= "<script type=\"text/javascript\">
				var form_".$form_id." = new FormManager('$form_id', '$callback_filename');
				form_".$form_id.".loadSections();
			</script>";

		return $form;
	}

	function validate() {
	}
}

define('SECTION_ACTION_NEXT', 's_action_next');
define('SECTION_ACTION_PREV', 's_action_prev');
define('SECTION_ACTION_SAVE', 's_action_save');
class section {

	public  $name;
	public  $id;
	public  $parent_id;
	private $form_fields_ar;
	private $actions_ar;
		
	private $attibutes_ar;

	function section($name, $id, $parent_id) {
		$this->name      = $name;
		$this->id        = $id;
		$this->parent_id = $parent_id;
				
		$this->form_fields_ar = array();
		$this->actions_ar     = array();
		
		$this->attributes_ar = array();
	}

	function getId() {
		return $this->id;
	}

	function setAttribute($name, $value){
		$this->attributes_ar[$name] = $value;
	}

	function addField($field) {
		if (!isset($this->form_fields_ar[$field->name])) {
			$this->form_fields_ar[$field->name] = $field;
		} //else {
			//echo 'FIELD '.$field->name.' IS ALREADY SET. ';
		//}
	}

//	//
//	//	!!! Add action should add an object not use the name...
//	//
	function addAction($action_name, $action) {
		$this->actions_ar[$action_name] = $action;
	}
	
	function addDescription($descr) {
		$this->form_fields_ar[] = '<p>'.$descr.'</p>';
	}
	
	function getOutput() {
		
		$atrb = '';
		foreach($this->attributes_ar as $name => $value) {
			$atrb .= $name.' = "'.$value.'" ';
		}
		
		$section_html = '<div id="'.$this->id.'" '.$atrb.'>';
		if (isset($this->name)) $section_html .= '<h3>'.$this->name.'</h3>';
		foreach($this->form_fields_ar as $field) {
			$section_html .= '<br>'.$field->getOutput();
		}
		foreach($this->actions_ar as $action) {
			$section_html .= '<br>'.$action->getOutput();
		}
		$section_html .= '</div>';
		
		return $section_html;
	}
}

// DATA LIST CLASSES

/**
* fileList provides a set of methods to create smart file lists.
* Requires: Class action
*/
class fileList {

	//settings
	private $directories_ar;
	private $id;
	private $title;
	private $is_hot;
	private $is_delete;
	private $display_size;
	private $display_index;
	private $display_image_title;
	private $display_subdirs;
	private $file_filters_ar;
	private $anchor_params_ar;
	private $attributes_ar;
	private $thumb_width;
	private $thumb_height;

	function __construct($id, $baseDirectory, $title=null) {
		$this->id = $id;
		$this->directories_ar = array($baseDirectory);
		$this->title = $title;

		$this->is_hot = true;
		$this->is_delete = false;
		$this->display_size = false;
		$this->display_index = false;
		$this->display_image_title = true;
		$this->display_thumbs = false;
		$this->display_subdirs = false;
		$this->file_filters_ar = array();
		$this->anchor_params_ar = array();
		$this->attributes_ar = array();

	}

	function setTitle($title) {
		$this->title = $title;
	}

	// Set the list of files as hyperlinks
	function setHot($is_hot=true) {
		$this->is_hot = $is_hot;
	}

	// do we allow files to be deleted?
	function setDelete($delete=false) {
		$this->is_delete = $delete;
	}

	// do we display the size of each file?
	function displaySize($display_size=true) {
		$this->display_size = $display_size;
	}

	// do we display an index for each file?
	function displayIndex($display_index=true) {
		$this->display_index = $display_index;
	}
	
	function displayImageTitle($display=true) {
		$this->display_image_title = $display;
	}

	// do we display thumbnails if file is image?
	function displayThumbs($display_thumbs=true) {
		$this->display_thumbs = $display_thumbs;
	}

	// do we display thumbnails if file is image?
	function displaySubDirs($display_subdirs=true) {
		$this->display_subdirs = $display_subdirs;
	}

	// file type filter
	function addFileFilter($extension) {
		$this->file_filters_ar[] = $extension;
	}

	// add a directory from which to list more files
	function addFileDirectory($directory) {
		$this->directories_ar[] = $directory;
	}

	// what parameters do want for the <a> tag?
	function setAnchorParam($name, $value) {
		$this->anchor_params_ar[$name] = $value;
	}

	function setAttribute($name, $value){
		$this->attributes_ar[$name] = $value;
	}

	function setThumbSize($width=null, $height=null) {
		$this->thumb_width  = $width;
		$this->thumb_height = $height;
	}
	
	function getOutput() {

		// get attributes for div tag
		$atrb = '';
		foreach($this->attributes_ar as $name => $value) {
			$atrb .= $name.' = "'.$value.'" ';
		}

		// get anchor parameters
		if ($this->is_hot) {
			$base_anchor = new action(ACTION_GOTO);
			$prms = ' ';
			foreach($this->anchor_params_ar as $name => $value) {
				$base_anchor->setAttribute($name, $value);
			}
		}

		$sub_dirs_ar = array();

		foreach($this->directories_ar as $dirname) {

			$file_list = '<table id="'.$this->id.'"><tr><td>';
			if (isset($this->title)) $file_list .= "<h3>".$this->title."</h3>";
			
			$files = array();
			if ($dp = @opendir($dirname)) {
				while (($file = readdir($dp)) !== false) {
					if ($file == "." || $file == "..") continue;
					array_push($files, $file);
				}
			}
			
			$index = 0;
			if ( count($files) > 0 ) {
				
				array_multisort($files, SORT_ASC);
				
				foreach ($files as $file) {
					$tmp = strtolower($file);
					$ext = strrchr($tmp,".");
					$image_title = substr($file, 0, strrpos($file, "."));
					$image_title = str_replace('_', ' ', $image_title);
					if (!in_array($ext, $this->file_filters_ar)) {
						$pathname = $dirname.'/'.$file;

						//store any subdirectories
						if ($ext == '') {
							$sub_dirs_ar[$file] = $pathname;
							break;
						}

						$index++;

						//get file size
						if ($this->display_size) {
							$statvar = stat($pathname); //stat returns variables about file in an array, at index 7 the file size is stored, @ 	avoids having error message if stat fails..
							$size = (($statvar[7]))/1024;
							$size = round($size);
							clearstatcache();
						}
						
						//gen html
						$file_list .= '<div id="'.$this->id.'_'.$index.'" class="fli" '.$atrb.'>';
						if ($this->display_index) {
								$file_list .= '<span class="index">';
								$file_list .= $index.'';
								$file_list .= '</span>';
						}

						if ($this->is_hot) {
							$anchor = $base_anchor; //grab a copy
							$anchor->setDestination($pathname);
							$anchor->setLabel($file);
							if($this->display_thumbs && ($ext == '.gif' || $ext == '.jpg' || $ext == '.bmp')) {
								$anchor->setImage($pathname);
								if (isset($this->thumb_width)) $anchor->setImageAttribute('width',  $this->thumb_width);
								if (isset($this->thumb_height)) $anchor->setImageAttribute('height', $this->thumb_height);
								$anchor->setImageAttribute('border', '1');
							}
							$file_list .= $anchor->getOutput();
							
							if ($this->display_image_title) {
								$file_list .= '<span class="image_title">';
								$file_list .= $image_title.'';
								$file_list .= '</span>';
							}
							
						} else {
							$file_list .= $file;
						}

						$del_action = new action(ACTION_DELETE, $pathname);
						$del_action->setLabel('[delete]');
						$del_action->setScript("onclick", 'fileManager_'.$this->id.'.deleteFile(\''.$this->id.'_'.$index.'\', \''.$pathname.'\'); return false;');
						if ($this->is_delete) {
								
								// Enable delete only if member is logged in.
								$login_app = new login();
								if ($login_app->validateLogin($_POST['user'], $_POST['pass'])) {
										$file_list .= '<span class="delete">';
										$file_list .= ' '.$del_action->getOutput();
										$file_list .= '</span>';
								}
						}
						if ($this->display_size) {
								$file_list .= '<span class="size">';
								$file_list .= ' '.$size.'kB';
								$file_list .= '</span>';
						}
						$file_list .= '</div>';
					} elseif ($ext == '') {
						$dir_list .= 'directory...';
					}
				}
			}

			// now add any sub directory
			if ($this->display_subdirs) {
				$file_list .= '<p>';
				foreach($sub_dirs_ar as $name => $pathname){
					$anchor = $base_anchor;
					$anchor->setDestination($pathname);
					$anchor->setLabel($name);
					$file_list .= $anchor->getOutput();
				}
				$file_list .= '</p>';
			}

			$file_list .= '</td></tr></table>';
			$filemanager_id = $this->id;
			echo <<<SCRIPT
				<script type="text/javascript">
					fileManager_$filemanager_id = new FileManager('$filemanager_id');
				</script>
SCRIPT;
			return $file_list;
		}
	}
}

class fileUpload {
	
	private $destination;
	
    function __construct($destination='root') {
		
		$this->destination = $destination;
	}
	
	function fileUploadForm($name, $action, $file_num) {
		
		// File upload access requires login
		$login_app = new login();
		if ($login_app->validateLogin($_POST['user'], $_POST['pass'])) {
		
			$form = '<h3>Upload a File:</h3>';
			$form .= '<form name="'.$name.'" action="'.$action.'" method="post" enctype="multipart/form-data">';
			for ($i=0; $i<$file_num; $i++) {
				$form .= 'File: <input name="userfile[]" type="file"><br>';
			}
			$form .= '<input type="hidden" name="action" value="'.ACTION_FILE_UPLOAD.'">';
			$form .= '<input type="submit" name="submit" value="Upload">';
			$form .= '</form>';
				
		    echo $form;
		}
	}
	
	function uploadFile() {
		
		if ( isset( $_FILES['userfile'] ) ) {
			
			$i=0;
			
			while( $i<count( $_FILES['userfile'] ) ){
				
				$file  = $_FILES ['userfile']['tmp_name'][$i];
				$fname = $_FILES['userfile']['name'][$i];
				$ftype = $_FILES['userfile']['type'][$i];
				$fname = strtr($fname, " \'\"\\", "____");
				//$fname = strtolower($fname);
				
				if ($fname != "") {
					if ($ftype == "application/octet-stream"){
						return false;
					} else {
						if ($fp = @fopen($file, "rb")){
							$fileSize = filesize($file);
							$contents = fread($fp,$fileSize);
							fclose($fp);
							$fp = fopen($this->destination.'/'.$fname,"wb");
							fwrite($fp, $contents);
							fclose($fp);
						} else {
							return false;
						}
					}
				}
				$i++;
			}
			
			$_FILES = array();
			header('Location: '.$_SERVER['REQUEST_URI']);
		}
	}
}

?>