#ifndef BRKS_COMMON_DATASEVER_SQLTABLES_H
#define BRKS_COMMON_DATASEVER_SQLTABLES_H

#include <memory>
#include "sqlconnection.h"
#include "glo_def.h"

class SqlTables
{
public:
	SqlTables(std::shared_ptr<MysqlConnection> sqlConn) :sqlconn_(sqlConn)
	{

	}

	bool CreatUserTable()
	{
		LOG_DEBUG("正在创建用户信息表......");
		const char* pUserInfoTable = "\
			CREATE TABLE IF NOT EXISTS userinfo( \
			id			 int(16)			NOT NULL primary key auto_increment, \
			mobile		 varchar(16)		NOT NULL default '13000000000', \	
			username	 varchar(18)	    NOT NULL default 'bikeuser', \
			userpassword varchar(16)    NOT NULL, \
			registertime timestamp		NOT NULL default CURRENT_TIMESTAMP, \
			lastUsetime  timestamp		NULL DEFAULT NULL, \
			money		 int(4)			NOT NULL default 10, \
			INDEX		 mobile_index(mobile) \
			)";
			if (!sqlconn_->Execute(pUserInfoTable))
			{
				LOG_ERROR("create table userinfo table failed. error msg:%s", sqlconn_->GetErrInfo());
				return false;
			}
		LOG_DEBUG("用户信息表创建成功！");

		return true;
	}

	bool CreateBikeTable()
	{
		LOG_DEBUG("正在创建单车信息表......");
		const char* pBikeInfoTable = " \
						CREATE TABLE IF NOT EXISTS bikeinfo( \
						id			int				NOT NULL primary key auto_increment, \
						devno		int				NOT NULL, \
						status		tinyint(1)		NOT NULL default 2, \
						trouble		int				NOT NULL default 0, \
						tmsg		varchar(256)	NOT NULL default '', \
						time		timestamp		NOT NULL default CURRENT_TIMESTAMP, \
						bike_point	point			NOT NULL	\
						)";


		//const char* pBikeInfoTable = "create table if not exists bikeinfo(id int, name char(6));";
		if (!sqlconn_->Execute(pBikeInfoTable))
		{
			LOG_ERROR("create bikeinfo table failed. error msg:%s", sqlconn_->GetErrInfo());
			return false;
		}
		LOG_DEBUG("单车信息表创建成功！");
		return true;
	}

private:
	std::shared_ptr<MysqlConnection> sqlconn_;
};


#endif 

