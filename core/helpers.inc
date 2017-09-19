<?php

// inserts $element at position $pos of
// ordered array and move all following 
// elements by one.
function &array_insert($array, $element, $pos) {
	$new_array = array_slice($array, 0, $pos);
	$new_array[] = $element;
	$new_array = array_merge($new_array, array_slice($array, $pos+1, count($array)));
	return $new_array;
}

function js_hash($array) {
	$js_array_str = '{';
	$i = 0;
	foreach($array as $key => $val) {
		$js_array_str .= $key.':\''.$val.'\'';
		$i++;
		if (sizeof($array) > $i) $js_array_str .= ',';
	}
	$js_array_str .= "}";
	return $js_array_str;
}

function js_array($array) {
	$js_array_str = '[';
	$i = 0;
	foreach($array as $val) {
		$js_array_str .= "'$val'";
		$i++;
		if (sizeof($array) > $i) $js_array_str .= ',';
	}
	$js_array_str .= "]";
	return $js_array_str;
}

function url_serialize($array) {
	$string = '';
	$i=0;
	foreach($array as $key => $val) {
		$string .= "$key=$val";
		$i++;
		if (sizeof($array) > $i) $string .= '&';
	}
	return $string;
}

?>
