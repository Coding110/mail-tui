<?php

require_once  'plog/classes/plog.php';
Plog::set_config(include 'plog/config.php');
$log = Plog::factory(__FILE__);
include 'DBHelper.php';

//初始化日志信息

  
  class qqServer{

  	 
  	/**
  	 * 分配数据--号码
  	 * API:
  	 * http://host/ws.php?method=get-qq-numbers&u=username&version=1.0
  	 * Return:{
  	 *   "version":1.0,
  	 *   "QQs":"50000,50001,..." # 每个号由逗号隔开，数量由实际情况决定
  	 *   }
  	 * 
  	 */
  	public function allocatNum(){
  		global $log;
  	    $log->debug("开始分配QQ号段信息");
  		//查询当前QQ max 进行分配
  		$db = new DBHelper();
  		$sql = "select   * from max_allocated_qq";
  		$res = $db->excuteSql($sql,'QUERY');
  		$resObj = $res->fetch_object(); 
  		
  		if($resObj ==NULL){
  			$log->debug("QQ信息不存在");
  		}else{
  			$startNumber = $resObj->number;//起始QQ号码
  			$allocCount = $resObj->allocCount ;//分配个数
  			$endNumber = $startNumber+$allocCount-1;
  			$log->debug("当前QQ号码:".$startNumber."分配个数:".$allocCount);
  			$qqString = '';
  			
  			//json格式数据客户端请求
  			for($i=$startNumber;$i<=$endNumber;$i++){
  				if($i==$endNumber){
  					$qqString.=$i;
  				}else{
  					$qqString .=$i.',';
  				}
  			}
  			//相应json数据
  			$resArr = array('version'=>'1.0','QQs'=>$qqString);
  			$json= json_encode($resArr); //转换为json数据
  			
  			$db->close();
  			$log->debug("响应客户端数据信息:".$json);
  			echo($json);
  			//更新数据信息
  		    $this->updateAllocNum($startNumber,$allocCount);
  		  	
  		}
  		
  		
  	}
  	
  	
  	/**
  	 *  更新分配QQ号段信息
  	 */
  	public static function updateAllocNum($startNumber,$allocCount){
  		global $log;
  		
  		//更新已分配QQ号段信息 以及 存储 已分配号段信息
  		$db = new DBHelper();
  		$endNumber = $startNumber+$allocCount;
  		
  		//更新当前QQ号段表
  		$sql = "update max_allocated_qq set number=".$endNumber;
  		$log->debug("开始更新表 信息max_allocated_qq:".$sql);
  		$res = $db->excuteSql($sql,'UPDATE');
  		
  		//临时保存已分配的QQ号码
  		$qqString = '';
  		for($i=$startNumber;$i<=$endNumber;$i++){
  			$alloc_time =  date("Y-m-d H:i:s");
  			if($i==$endNumber){
  				$qqString .="('".$i."','".$alloc_time."')";
  			}else{
  				$qqString .="('".$i."','".$alloc_time."'),";
  			}
  			
  		}
  		
  		$sqlTemp = "insert allocated_qq (number,alloc_time) values ".$qqString;
  		$log->debug("更新已分配表信息allocated_qq:".$sqlTemp);
  		$res = $db->excuteSql($sqlTemp,'ADD');
  		$db->close();
  	
  	}
  	
  	/**
  	 * 存储客户端上传数据--号码
  	 * API:
  	 * http://host/ws.php?method=upload-qq-result&u=username&version=1.0
  	 * DATA:
  	 * {
  	 *    "version":1.0,
  	 *     "result":[
  	 *     {"qq":"50000", "weight": 2},
  	 *     {"qq":"50001", "weight": 0}
  	 *   ]
  	 * }
  	 * 
  	 */
  	public function saveNum($jsonArrQQ){
  		global $log;
  		
  		$db = new DBHelper();
  		foreach ($jsonArrQQ as $keyQQ){
  			
  			$qqNum = $keyQQ->qq;
  			$weight = $keyQQ->weight;//权值
  			$detect_time = date("Y-m-d H:i:s");
  			
  			
  			//保存上传结果
  			$sql = "insert active_qq (number,weight,detect_time) values('".$qqNum."','".
  			$weight."','".$detect_time."')";
  			$log->debug("上传QQ信息表 sql active_qq:".$sql);
  			$db->excuteSql($sql,'Add');
  		}
  		
  		
  		//保存上传QQ个数
  		$ip = get_client_ip();
  		//客户端名字
  		$name = '';
  		if(isset($_GET['username'])){
  			$name = $_GET['username'];
  		}
  		$qq_active_count=count($jsonArrQQ);//活跃的QQ个数
  		$sql = "insert client_detected_log (name,qq_active_count,ip) values('".$name."','".
  				$qq_active_count."','".$ip."')";
  		
  		$log->debug("记录上传QQ个数 sql:".$sql);
  		$db->excuteSql($sql,'Add');
  		$db->close();
  		
  	}
  	
  	
  	
  }
  
  $qqSerObj = new qqServer();
  $methodName = $_GET['method'];
  
 
  //$jsonArr = json_encode($jsonString);
  
  
  /**
   *分配QQ号码
   **/
  if(isset($methodName)&&$methodName=='get-qq-numbers'){
  	$log->debug("开始执行方法:".$methodName);
  	$qqSerObj->allocatNum();
  }
  
  
  
  /**
   *处理上传结果
   **/
  if(isset($methodName)&&$methodName=='upload-qq-result'){
  	//$jsonString = '{"version":"1.0","result":[{"qq":"12345","weight":2},{"qq":"233232","weight":1}]}';
  	//将数组或者对象转换json格式
  	
  	if(isset($_REQUEST['result'])){
  		$jsonString=$_REQUEST['result'];
  		$log->debug("收到的json数据:".$jsonString);
  		$jsonArr = json_decode($jsonString);
  		$log->debug("客户端版本号:".$jsonArr->version);
  		//收到QQ数组
  		
  		$jsonArrQQ = $jsonArr->result;
  		$log->debug('开始执行方法:'.$methodName);
  		$qqSerObj->saveNum($jsonArrQQ);
  	}
  	
  }
  
  
  /**
   * 获取用户的真实ip地址
   * @return string
   */
  function get_client_ip(){
  	$headers = array('HTTP_X_REAL_FORWARDED_FOR', 'HTTP_X_FORWARDED_FOR', 'HTTP_CLIENT_IP', 'REMOTE_ADDR');
  	foreach ($headers as $h){
  		$ip = $_SERVER[$h];
  		// 有些ip可能隐匿，即为unknown
  		if ( isset($ip) && strcasecmp($ip, 'unknown') ){
  			break;
  		}
  	}
  	if( $ip ){
  		// 可能通过多个代理，其中第一个为真实ip地址
  		list($ip) = explode(', ', $ip, 2);
  	}
  	/* 如果是服务器自身访问，获取服务器的ip地址(该地址可能是局域网ip)
  	 if ('127.0.0.1' == $ip){
  	$ip = $_SERVER['SERVER_ADDR'];
  	}
  	*/
  	return $ip;
  }
  
	
?>