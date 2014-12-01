CREATE DATABASE IF NOT EXISTS test_email default charset utf8;
use test_email;
-- 已分配QQ中最大的QQ，由小递增分配QQ给客户端 allocCount 分配个数
DROP  TABLE  IF EXISTS max_allocated_qq;
CREATE TABLE max_allocated_qq(
    number BIGINT NOT NULL
	
)ENGINE=MyISAM DEFAULT CHARSET=utf8;
 
-- 已分配给客户端，但未上传探测结果的QQ。对alloc_time超时1小时的重新分配，整点检查此表。
DROP TABLE IF EXISTS allocated_qq;
CREATE TABLE allocated_qq(
    id BIGINT NOT NULL AUTO_INCREMENT,
    number BIGINT NOT NULL,
    alloc_time datetime,
    PRIMARY KEY (id)
	  
)ENGINE=MyISAM DEFAULT CHARSET=utf8;
 
-- 探测完成且活跃度大于0的QQ
DROP TABLE IF EXISTS active_qq;
CREATE TABLE active_qq(
    id BIGINT NOT NULL AUTO_INCREMENT,
    number BIGINT NOT NULL,
    weight INT,
    detect_time datetime,
		PRIMARY KEY (id)
)ENGINE=MyISAM DEFAULT CHARSET=utf8;
 
-- 客户端探测QQ的数量日志
DROP TABLE IF EXISTS client_detected_log;
CREATE TABLE client_detected_log(
    id BIGINT NOT NULL AUTO_INCREMENT,
    name VARCHAR(255),
    qq_detected_count BIGINT,
    qq_active_count BIGINT,
    ip VARCHAR(40) ,
		PRIMARY KEY (id)
)ENGINE=MyISAM DEFAULT CHARSET=utf8;