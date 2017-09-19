<?php
require_once('config.inc');
/**
* Data models
*/


class db_connect {
	
	private $db_link;
	private $error_msg;

	private $query_offset; // fetch single offset

	function __construct() {

		// the offset to be used by fetches
		$this->query_offset = 0;

		$this->db_link = mysql_connect(DBHOST, DBLOGIN, DBPASS);
		if(!mysql_select_db(DB, $this->db_link)) {
			$this->error_msg = "Unable to access data.";
		}
	}

	function &fetch_single($query) {
		$query .= ' LIMIT 1 OFFSET '.$this->query_offset;
		$result = mysql_query( $query, $this->db_link );
		$rowarray = mysql_fetch_row($result);
		$this->query_offset++;
		return $rowarray;
	}

	function &fetch_many($query, $sort=SORT_ASC) {
		$result = mysql_query($query, $this->db_link);
		$row = mysql_fetch_row($result);
		$TDAr_many = array();
		while ($row!=FALSE){
			array_push($TDAr_many, $row);
			$row = mysql_fetch_row($result);
		}
		$ord= array();
		foreach($TDAr_many as $key => $val){
			$ord[$key] = $val[0];
		}
		@array_multisort($ord, $sort, $TDAr_many);
		return $TDAr_many;
	}

	function insert($query) {
		$result = mysql_query($query, $this->db_link);
		return $result;
	}

	function db_close() {
		@mysql_close($this->db_link);
	}
}

?>