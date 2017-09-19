<?php

/** A pane regroups a set of applications.
 * A pane can be drawn upon an action request,
 * and typically replaces another pane.
*/
class pane extends substrate {
	
	public $name;
	public $application_ar;
	public $app_name_list;
	public $div_class;
		
	function __construct($name) {
		parent::__construct();
		$this->name = $name;
		$this->application_ar = array();
		$this->app_name_list  = array();
		$this->div_class = 'pane';
	}
	
	function addApplication($application) {
		$this->application_ar[$application->getId()] = $application;
		array_push($this->app_name_list, get_class($application));
	}
	
	function getName() {
		return $this->name;
	}
	
	function setDiv($div_class) {
		$this->div_class = $div_class;
	}
	
	function getOutput() {
		$output = '<div id="'.$this->getId().'" class="'.$this->div_class.'">';
		foreach($this->application_ar as $object_id => $object) {
			$output .= $object->getOutput();
		}
		$output .= '</div>';
		return $output;
	}

}

?>