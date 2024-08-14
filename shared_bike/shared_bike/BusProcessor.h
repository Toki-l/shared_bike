#ifndef BRKS_BUS_MAIN_H
#define BRKS_BUS_MAIN_H

#include "user_event_handler.h"
#include "sqlconnection.h"
#include <memory>


class BusinessProcessor
{
public:
	BusinessProcessor(std::shared_ptr<MysqlConnection> conn);
	bool init();
	virtual ~BusinessProcessor();

private:
	std::shared_ptr<MysqlConnection> mysqlconn_;
	std::shared_ptr<UserEventHandler> ueh_;

};


#endif
