<?php
     include 'DBHelper.php';
     $db = new DBHelper();
     
     $sql = "insert user (username,pwd) values('张三','123456') ";
     
     $db->excuteSql('select * from user','Query');
     
     //$db->close();
	

?>

