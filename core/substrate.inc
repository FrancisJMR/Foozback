<?php

class substrate {
	
	public $public_id;
	private $id;
	
	private $input_ar;
	private $output;
	
	function __construct() {
		
		global $gid;
		if(!isset($gid)) $gid = 0;
		$gid++;
		$this->id = $gid;
		if ($this instanceof substrate) $this->public_id = $this->id;
		else $this->public_id = $this->id + parent::getPublicId();
	}
	
	function __destruct() {
		unset($this);
	}
	
	function getId() {
		return $this->id;
	}
	
	function getPublicId() {
		return $this->public_id;
	}
	
	function setInput($input_ar) {
		foreach($input_ar as $item_name => $item) {
			$this->{$item_name} = $item;
		}
	}
	
	function prepare() {
		
		$this->output = serialize($this->input_ar);
	}
	
	function getOutput() {
		return $this->output;
	}
}

class inputField extends substrate {
	
	private $id;
	public $name;
	private $label;
	private $type;
	private $value;
	private $extra;
	
	function __construct ($name, $label=null, $type=null, $value=null, $extra=null) {

		parent::__construct();
		
		$this->id    = $this->public_id.'_'.$name;
		$this->name  = $name;
		$this->label = $label;
		$this->type  = $type;
		$this->value = $value;
		$this->extra = $extra;
	}

	function getOutput() {
		$output = '';
		if (isset($this->label) && strtolower($this->type) != 'hidden') $output .= '<LABEL FOR="'.$this->getId().'">'.$this->label.'</LABEL>&nbsp;:&nbsp;';
		$output .= '<INPUT ';
		if (isset($this->type)) { 
			$output .= 'TYPE="'.$this->type.'" ';
		} else {
			$output .= 'TYPE="text" ';
		}
		$output .= 'ID="'.$this->getId().'" ';
		$output .= 'NAME="'.$this->name.'" ';
		if (isset($this->value)) $output .= 'VALUE="'.$this->value.'" ';
		if (isset($this->extra)) $output .= $this->extra;
		$output .= '>';
		return $output;
	}
}

class radioField extends substrate {
	
	private $id;
	public $name;
	public $group_name;
	public $value;
	private $label;
	private $extra;
	
	private $checked;
	
	function __construct ($name, $group_name, $value, $label=null, $extra=null) {

		parent::__construct();
		
		$this->id    = $this->public_id.'_'.$name;
		$this->name  = $name;
		$this->group_name = $group_name;
		$this->label = $label;
		$this->value = $value;
		$this->extra = $extra;
		
		$this->checked = FALSE;
	}

	function setChecked($checked=TRUE){
		$this->checked = $checked;
	}
	
	function getOutput() {
		$output = '';
		if (isset($this->label)) $output .= '<LABEL FOR="'.$this->getId().'">'.$this->label.'</LABEL>&nbsp;:&nbsp;';
		$output .= '<INPUT ';
		$output .= 'TYPE="radio" ';
		$output .= 'ID="'.$this->getId().'" ';
		$output .= 'NAME="'.$this->group_name.'" ';
		$output .= 'VALUE="'.$this->value.'" ';
		if ($this->checked) $output .= 'CHECKED="checked" ';
		if (isset($this->extra)) $output .= $this->extra;
		$output .= '>';
		return $output;
	}
}

class selectField extends substrate {
	
	private $id;
	public $name;
	private $label;
	private $selected_option;
	private $extra;
	private $options_ar;
	
	function __construct ($name, $label=null, $selected_option=null, $extra=null) {

		parent::__construct();
		
		$this->id    = $this->public_id.'_'.$name;
		$this->name  = $name;
		$this->label = $label;
		$this->selected = $selected;
		$this->extra = $extra;
		
		$this->options_ar = array();
	}

	function addOption($value, $name) {
		if (!isset($this->options_ar[$value])){
			$this->options_ar[$value] = $name;
		} else {
			echo 'OPTION '.$value.' IS ALREADY SET. ';
		}
	}
	
	function getOutput() {
		$output = '';
		if (isset($this->label)) $output .= '<LABEL FOR="'.$this->getId().'">'.$this->label.'</LABEL>&nbsp;:&nbsp;';
		$output .= '<SELECT ';		
		$output .= 'ID="'.$this->getId().'" ';
		$output .= 'NAME="'.$this->name.'" ';
		if (isset($this->extra)) $output .= $this->extra;
		$output .= '>';
		
		foreach ($this->options_ar as $value => $name){
			if ($selected_option == $value) {
				$output .= '<OPTION VALUE="'.$value.'" SELECTED="selected">'.$name.'</OPTION>';
			} else {
				$output .= '<OPTION VALUE="'.$value.'">'.$name.'</OPTION>';
			}
		}
		
		$output .= '</SELECT>';
		return $output;
	}
}

class textareaField extends substrate {
	
	private $field;
	private $id;
	public $name;
	private $label;
	private $text;
	private $rows;
	private $cols;
	private $extra;
	
	function __construct($name, $label=null, $text=null, $rows=null, $cols=null, $extra=null) {
		
		parent::__construct();
		
		$this->id    = $this->public_id.'_'.$name;
		$this->name  = $name;
		$this->label = $label;
		$this->text  = $text;
		$this->rows  = $rows;
		$this->cols  = $cols;
		$this->extra = $extra;
	}
	
	function getOutput () {
		$text_area = '';
		if (isset($this->label)) $text_area .= '<LABEL FOR="'.$this->getId().'">'.$this->label.'</LABEL>&nbsp;:&nbsp;';
		$text_area .= '<TEXTAREA ';
		$text_area .= 'ID="'.$this->getId().'" ';
		$text_area .= 'NAME="'.$this->name.'" ';
		if (isset($this->rows))  $text_area .= 'ROWS="'.$this->rows.'" ';
		if (isset($this->cols))  $text_area .= 'COLS="'.$this->cols.'" ';
		if (isset($this->extra)) $text_area .= $this->extra;
		$text_area .= '>';
		if (isset($this->text)) $text_area .= $this->text;
		$text_area .= '</TEXTAREA>';
		return $text_area;
	}
}

class buttonField extends substrate {
	
	private $id;
	public $name;
	private $label;
	private $value;
	private $script;
	
	function __construct($name, $label, $value, $type=null, $script=null) {
		
		parent::__construct();
		
		$this->id = $this->public_id.'_'.$name;
		$this->name = $name;
		$this->label = $label;
		$this->value = $value;
		$this->type = $type;
		$this->script = $script;
	}
	
	function getOutput() {
		
		if($this->type == 'submit') $button = '<INPUT ';
		else $button = '<BUTTON ';
		$button .= 'ID="'.$this->getId().'" ';
		$button .= 'NAME="'.$this->name.'" ';
		if(isset($this->type)) {
			$button .= 'TYPE="'.$this->type.'" ';
		} else {
			$button .= 'TYPE="submit" ';
		}
		if(isset($this->value))   $button .= 'VALUE="'.$this->value.'" ';
		if (isset($this->script)) $button .= 'ONCLICK="'.$this->script.'"';
		
		$button .= '>';		
		if($this->type != 'submit') {
			$button .= $this->label;
			$button .= '</BUTTON>';
		}		
		return $button;
	}
}
?>