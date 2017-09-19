<?php

require_once('models.inc');
require_once('substrate.inc');
require_once('helpers.inc');

class application extends substrate {

	public $objects_ar;
	public $div_class;

	function __construct() {
		parent::__construct();
		$this->objects_ar = array();
		$this->div_class = 'application';
	}

	function setDiv($div_class) {
		$this->div_class = $div_class;
	}

	function addObject($object) {
		$this->objects_ar[$object->getId()] = $object;
	}

	function getObject($obj_id) {
		return $this->objects_ar[$obj_id];
	}

	function prepare() {
		foreach($this->objects_ar as $object_id => $object) {
			$object->prepare();
		}
	}

	function getOutput(){

		$output = '<div id="'.$this->getId().'" class="'.$this->div_class.'">';
		foreach($this->objects_ar as $object_id => $object) {
			$output .= $object->getOutput();
		}
		$output .= '</div>';
		return $output;
	}
}


/** High order Application which allows to
 * load Panes of applications in the target area.
*/
class menu extends application {
	
	public $default_pane;
	public $panes_ar;
	public $target_id;
	
	function __construct() {
		parent::__construct();
		$this->div_class = 'menu';
		$this->default_pane = null;
		$this->links_ar = array();
		$this->target_id = 'target'.$this->getId();
	}
	
	function addPane($pane) {
		// store pane
		$this->panes_ar[$pane->getId()] = $pane;
		// add menu link
		$pane_link = new action(ACTION_PANE);
		$pane_link->setLabel($pane->getName());
		$pane_link->setAttribute('id', $pane_link->getId());
//		$jshash_pane_apps = js_hash($pane->app_name_list);
		$app_serialized = $pane->app_name_list;
		$app_serialized = array_merge($app_serialized, array('action'=> ACTION_PANE));
		$url_serialized_apps = url_serialize($app_serialized);
//		echo $url_serialized_apps;
		$pane_link->setScript('onclick', 'menu_'.$this->getId().'.loadPane(\''.$url_serialized_apps.'\'); return false;');
		$this->addObject($pane_link);
	}
	
	// $default_target is a Pane or Apps
	function setMenuDefaultPane($default_pane) {
		$this->default_pane = $default_pane;
	}
	
	function getOutput(){
		$output = '<div id="'.$this->getId().'" class="'.$this->div_class.'">';
		foreach($this->objects_ar as $object_id => $object) {
			if ($object === $this->default_pane) {
				$output .= '<li><b>'.$object->getOutput().'</b>';
			} else {
				$output .= '<li>'.$object->getOutput();
			}
		}
		
		// Maybe the following should only appear on callback...
		foreach ($this->panes_ar as $pane_id => $pane) {
			$output .= $pane->getOutput();
		}
		// end
		
		$output .= '</div>';
		if (isset($this->default_pane)) $output .= '<div id="'.$this->target_id.'">'.$this->default_pane->getOutput().'</div>';
		else $output .= '<div id="'.$this->target_id.'"></div>';
		
		$output .= "<script type=\"text/javascript\">
				var menu_".$this->getId()." = new MenuManager('".$this->getId()."', '".$this->target_id."');
				</script>";
		return $output;
	}
}


/** Displays a Login application
 * Sets _SESSION user and pass variables
 * once loged in.
*/
class login extends application {

	function __construct() {		
		parent::__construct();
		$this->start_session();
		$_SESSION['reqlogin'] = true;
	}

	function start_session() {
		session_start();
	}

	function buildRegisterForm($div_class=null) {
		$login_form = new form('index.php', 'post', 'login_form');

		//login input
		$user_input_field = new inputField('user', 'Username:');
		$login_form->addField($user_input_field);
		//password input
		$pass_input_field = new inputField('pass', 'Password:', 'password');
		$login_form->addField($pass_input_field);
		//email input
		$email_input_field = new inputField('email', 'Email:');
		$login_form->addField($email_input_field);

		$this->addObject($login_form);
	}

	function insertUserLogin($user, $pass, $email, $status) {
		$dbc = new db_connect();
		$row = $dbc->fetch_single('select pk from member_status where name = $status');
		$status_pk = $row['pk'];
		$result = $dbc->insert("insert into members ('user', 'pass', 'email', 'status_pk') values ('$user', '$pass', '$email', $status_pk)");
		$dbc->db_close();
		return $result;
	}

	function buildLoginForm() {
		
		$login_app = new login();
		
		if ($login_app->validateLogin($_POST['user'], $_POST['pass'])) {
			
			$logout_action = new action(ACTION_LOGOUT);
			$logout_action->setLabel('Logout');
			$this->addObject($logout_action);
			
		} else {
			
			$login_form = new form('index.php', 'post', 'login_form');
			
			//login input
			$user_input_field = new inputField('user', 'Username');
			$login_form->addField($user_input_field);
			//password input
			$pass_input_field = new inputField('pass', 'Password', 'password');
			$login_form->addField($pass_input_field);
			
			$action_input_field = new inputField(FORM_ACTION, null, 'hidden', 'login');
			$login_form->addField($action_input_field);
			
			$this->addObject($login_form);
		}
	}

	function validateLogin($user=null, $pass=null) {
		//session_destroy();
		$user = isset($user) ? $user : $_SESSION['user'];
		$pass = isset($pass) ? $pass : $_SESSION['pass'];

		$dbc = new db_connect();
		$row = $dbc->fetch_single("select m.pass, ms.name from members m, member_status ms where m.user = '$user'");
		
		//if(sha1($pass)===$row['pass']){ //authenticated
		if(isset($row[0]) && $pass === $row[0]){ //authenticated
			$_SESSION['user'] = $user;
			//$_SESSION['pass']=sha1($pass);
			$_SESSION['pass'] = $pass;
			$_SESSION['status'] = $row[1];
			$dbc->db_close();
			
			return true;
		}
		$dbc->db_close();
		return false;
	}

	function logout() {
	}
}

/** NON CORE
 * This is not a core application
 * Just a trial file listing applicatoin with
 * hard build params.
 */
class fileDeleteList extends application {
	
	public $file_list;
	
	function __construct($title, $dir) {
		parent::__construct();
		$this->file_list = 	new fileList('list_'.$this->getId(), $dir);
		$this->file_list->setTitle('Demo File List');
		$this->file_list->displayIndex(true);
		$this->file_list->setDelete(true);
		$this->file_list->displaySize(true);
		$this->file_list->displaySubDirs(true);
	}
	
	function getOutput() {
		$this->file_list->getOutput();
	}
	
}

/** NON CORE
 * This is not a core application
 * Just a test form
 */
class sectionForm extends application {
	
	public $form;
	
	function __construct($action, $method='post', $id='form') {
		parent::__construct();
		
		$this->form = new form($action, $method, $id);
	}
	
	function prepare() {
		//section 1
		$input_field = new inputField('pname', 'Product 1');
		// addField($field, $section_name=null)
		$this->form->addField($input_field, 'section1');
		
		$input_field = new inputField('pprice', 'Price');
		$this->form->addField($input_field, 'section1');
		
		$text_field = new textareaField('pdesc','Description');
		$this->form->addField($text_field, 'section1');
		
		
		//section 2
		$input_field = new inputField('p2name', 'Product 2');
		$this->form->addField($input_field, 'section2');
		
		$input_field = new inputField('name2','Price');
		$this->form->addField($input_field, 'section2');
		
		$text_field = new textareaField('name','Description');
		$this->form->addField($text_field, 'section2');
		
		
		//section 3
		$input_field = new inputField('uname', 'Your Name');
		$this->form->addField($input_field, 'section3');
		
		$text_field = new textareaField('comments','Comments');
		$this->form->addField($text_field, 'section3');
		
	}

	function getOutput() {
		return $this->form->getOutput();
	}
}

?>