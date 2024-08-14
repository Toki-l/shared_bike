#ifndef __BUSPROCESSOR_H_
#define __BUSPROCESSOR_H_

#include <memory>
#include "user_event_handler.h"
#include "sqlconnection.h"
#include "SqlTables.h"

class BusinessProcessor
{
public:
	BusinessProcessor(std::shared_ptr<MysqlConnection> sqlconn);
	virtual ~BusinessProcessor();

	bool init();

private:
	std::shared_ptr<MysqlConnection> mysqlconn_;
	//MysqlConnection* mysqlcon;
	std::shared_ptr<UserEventHandler> ueh_;
};


#endif // !__BUSPROCESSOR_H_
