<?php

/**
 * A group regroups core/extension application (e.g. section_form 
 * + nav action) together into a point-in-time user application. 
 * A group dissapears after navigation actions are performed from 
 * a menu or internal action.
 *
 */
class group {
	
	public $id;
	public $pane_id;
	
	private $applications_ar;
	
	function __construct($id, $pane_id=null) {
		$this->id = $id;
		$this->pane_id = $pane_id;
	}
	
	function addApplication($application) {
		$this->applications_ar[] = $application;
	}
	
	function setTargetPane($pane_id) {
		$this->pane_id = $pane_id;
	}
}

?>