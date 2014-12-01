CREATE DATABASE IF NOT EXISTS test_email default charset utf8;
use test_email;
-- �ѷ���QQ������QQ����С��������QQ���ͻ��� allocCount �������
DROP  TABLE  IF EXISTS max_allocated_qq;
CREATE TABLE max_allocated_qq(
    number BIGINT NOT NULL
	
)ENGINE=MyISAM DEFAULT CHARSET=utf8;
 
-- �ѷ�����ͻ��ˣ���δ�ϴ�̽������QQ����alloc_time��ʱ1Сʱ�����·��䣬������˱�
DROP TABLE IF EXISTS allocated_qq;
CREATE TABLE allocated_qq(
    id BIGINT NOT NULL AUTO_INCREMENT,
    number BIGINT NOT NULL,
    alloc_time datetime,
    PRIMARY KEY (id)
	  
)ENGINE=MyISAM DEFAULT CHARSET=utf8;
 
-- ̽������һ�Ծ�ȴ���0��QQ
DROP TABLE IF EXISTS active_qq;
CREATE TABLE active_qq(
    id BIGINT NOT NULL AUTO_INCREMENT,
    number BIGINT NOT NULL,
    weight INT,
    detect_time datetime,
		PRIMARY KEY (id)
)ENGINE=MyISAM DEFAULT CHARSET=utf8;
 
-- �ͻ���̽��QQ��������־
DROP TABLE IF EXISTS client_detected_log;
CREATE TABLE client_detected_log(
    id BIGINT NOT NULL AUTO_INCREMENT,
    name VARCHAR(255),
    qq_detected_count BIGINT,
    qq_active_count BIGINT,
    ip VARCHAR(40) ,
		PRIMARY KEY (id)
)ENGINE=MyISAM DEFAULT CHARSET=utf8;