#include "user_event_handler.h"
#include "DispatchMsgService.h"

#include <arpa/inet.h>
#include <assert.h>
#include <errno.h>
#include <netinet/in.h>
#include <signal.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <netdb.h>
#include <unistd.h>

UserEventHandler::UserEventHandler():iEventHandler("UserEventHandler")
{
	DispatchMsgService::getInstance()->subscribe(EEVENTID_GET_MOBILE_CODE_REQ, this);
	DispatchMsgService::getInstance()->subscribe(EEVENTID_LOGIN_REQ, this);
	thread_mutex_create(&pm_);
}

UserEventHandler::~UserEventHandler()
{
	DispatchMsgService::getInstance()->unsubscribe(EEVENTID_GET_MOBILE_CODE_REQ, this);
	DispatchMsgService::getInstance()->unsubscribe(EEVENTID_LOGIN_REQ, this);
	thread_mutex_destroy(&pm_);
}

iEvent* UserEventHandler::handle(const iEvent* ev)
{
	if (ev == NULL)
	{
		printf("input ev is NULL");
	}

	u32 eid = ev->get_eid();

	if (eid == EEVENTID_GET_MOBILE_CODE_REQ)
	{
		return handle_mobile_code_req((MobileCodeReqEv*)ev);
	}
	else if (eid == EEVENTID_LOGIN_REQ)
	{
		//return handle_login_req
	}
	else if (eid == EEVENTID_RECHARGE_REQ)
	{

	}
	else if (eid == EEVENTID_GET_ACCOUNT_BALANCE_REQ)
	{

	}
	else if (eid == EEVEBTID_LIST_ACCOUNT_RECORDS_REQ)
	{

	}

	return NULL;
}

MobileCodeRspEv* UserEventHandler::handle_mobile_code_req(MobileCodeReqEv* ev)
{
	i32 icode = 0;
	std::string mobile_ = ev->get_mobile();
	printf("try to get mobile phone %s validata code .\n", mobile_.c_str());

	icode = code_gen();

	thread_mutex_lock(&pm_);
	m2c_[mobile_] = icode;
	thread_mutex_unlock(&pm_);

	printf("mobile: %s, code: %d\n", mobile_.c_str(), icode);

	return new MobileCodeRspEv(200, m2c_[mobile_]);
}

i32 UserEventHandler::code_gen()
{
	i32 code = 0;
	srand((unsigned int)time(NULL));

	code = (unsigned int)(rand() % (999999 - 100000) + 100000);

	return code;
}