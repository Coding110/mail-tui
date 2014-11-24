-- 已分配QQ中最大的QQ，由小递增分配QQ给客户端
CREATE TABLE 'max_allocated_qq'(
    'number' BIGNIT NOT NULL
)ENGINE=MyISAM DEFAULT CHARSET=utf8;
 
-- 已分配给客户端，但未上传探测结果的QQ。对alloc_time超时1小时的重新分配，整点检查此表。
CREATE TABLE 'allocated_qq'(
    'id' BIGINT NOT NULL AUTO_INCREMENT,
    'number' BIGINT NOT NULL,
    'alloc_time' datetime
)ENGINE=MyISAM DEFAULT CHARSET=utf8;
 
-- 探测完成且活跃度大于0的QQ
CREATE TABLE 'active_qq'(
    'id' BIGINT NOT NULL AUTO_INCREMENT,
    'number' BIGINT NOT NULL,
    'weigth' INT,
    'detect_time' datetime
)ENGINE=MyISAM DEFAULT CHARSET=utf8;
 
-- 客户端探测QQ的数量日志
CREATE TABLE 'client_detected_log'(
    'id' BIGINT NOT NULL AUTO_INCREMENT,
    'name' VARCHAR(255),
    'qq_detected_count' BIGINT,
    'qq_active_count' BIGINT,
)ENGINE=MyISAM DEFAULT CHARSET=utf8;
