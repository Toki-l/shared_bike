#ifndef BRKS_BUS_USERM_HANDLER_H_
#define BRKS_BUS_USERM_HANDLER_H_

#include "glo_def.h"
#include "iEventHandler.h"
#include "event_def.h"
#include "threadpool/thread.h"

#include <string>
#include <map>
#include <memory>

//用户事件处理
class UserEventHandler : public iEventHandler
{
public:
	UserEventHandler();
	virtual~UserEventHandler();
	virtual iEvent* handle(const iEvent* ev);

private:
	MobileCodeRspEv* handle_mobile_code_req(MobileCodeReqEv* ev);
	i32 code_gen();		//模拟产生验证码

private:
	//std::string mobile_;
	std::map<std::string, i32> m2c_;	//first: mobile , second: code
	pthread_mutex_t pm_;
};





#endif // !BRKS_BUS_USERM_HANDLER_H_
