<?php
/**
   * @author HUA
   *
   */

require_once 'inc/db_config.php';

define( "UPDATE_OPERATE","UPDATE"); //更新操作
define( "ADD_OPERATE","UPDATE"); //插入操作
define( "QUERY_OPERATE","QUERY"); //插入操作
define( "DEL_OPERATE","DEL"); //删除操作
class DBHelper {

	/**
	 * @param unknown $host
	 * @param unknown $user
	 * @param unknown $pass
	 * @param unknown $db
	 */
	public function DBHelper() {
		global $host,$user,$pwd,$db;
		$this->mysqli = new mysqli($host,$user,$pwd,$db);
		if($this->mysqli ->connect_error){
			die("连接数据库失败： ".$this->mysqli ->connect_errno);
		}
		
		/*
		 * 设置字符集
		 * 
		 * */
		$this->mysqli ->query("set names utf8");
		
	}
	
	/**
	 * 执行sql语句函数
	 * 
	 * type 类型：
	 * Delete 删除
	 * Query 查询
	 * Update 修改
	 * Add 新增
	 * 
	 */
	public function excuteSql($sql,$type){
		//query 增 删返回TRUE OR FALSE 查询返回结果集
		$res = $this->mysqli->query($sql) or die("调用 excuteSql失败：".$this->mysqli->error);
		//如果是查询则返回查询结果数组
		if($type==QUERY_OPERATE){
			
// 			while($rowObj=$res->fetch_object()){
// 				echo "id:".$rowObj->id." username:".$rowObj->username." pwd:".$rowObj->pwd."\n";
// 			}
			
// 			while($row=$res->fetch_assoc()){
				
// 				echo "id:".$row['id']." username:".$row['username']." pwd:".$row['pwd']."\n";
// 			}
			
			return $res;
		}
		
		$this->mysqli->commit();		
		return $res;
	}


	
	/**
	 * 关闭数据库连接
	 */
	public function close(){
		$this->mysqli->close();
	}
}




?>
