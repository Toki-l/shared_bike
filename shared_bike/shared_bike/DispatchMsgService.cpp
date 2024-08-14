#include "DispatchMsgService.h"
#include "NetworkInterface.h"
#include "event_def.h"
#include <algorithm>

DispatchMsgService* DispatchMsgService::DMS_ = nullptr;
std::queue<iEvent*> DispatchMsgService::response_events;
pthread_mutex_t DispatchMsgService::queue_mutex;

DispatchMsgService::DispatchMsgService():tp(nullptr)
{
}

DispatchMsgService::~DispatchMsgService()
{
}

BOOL DispatchMsgService::open()
{
	svr_exit_ = FALSE;

	tp = thread_pool_init();
	thread_mutex_create(&queue_mutex);

	return tp?TRUE:FALSE;
}

void DispatchMsgService::close()
{
	svr_exit_ = TRUE;

	thread_pool_destroy(tp);
	thread_mutex_destroy(&queue_mutex);
	subscribers_.clear();

	tp = NULL;
}

void DispatchMsgService::subscribe(u32 eid, iEventHandler * handler)
{
	LOG_DEBUG("DispatchMsgService::subscribe eid: %u\n", eid);
	T_EventHandlersMap::iterator iter = subscribers_.find(eid);
	if (iter != subscribers_.end())
	{
		T_EventHandlers::iterator hdl_iter = std::find(iter->second.begin(), iter->second.end(), handler);
		if (hdl_iter == iter->second.end())
		{
			iter->second.push_back(handler);
		}

	}
	else
	{
		subscribers_[eid].push_back(handler);
	}
}

void DispatchMsgService::unsubscribe(u32 eid, iEventHandler * handler)
{
	T_EventHandlersMap::iterator iter = subscribers_.find(eid);
	if (iter != subscribers_.end())
	{
		T_EventHandlers::iterator hdl_iter = std::find(iter->second.begin(), iter->second.end(), handler);
		if (hdl_iter != iter->second.end())
		{
			iter->second.erase(hdl_iter);
		}

	}
}

i32 DispatchMsgService::enqueue(iEvent * ev)
{
	if (ev == NULL)
	{
		return -1;
	}

	thread_task_t* task = thread_task_alloc(0);

	task->handler = DispatchMsgService::svc;
	task->ctx = ev;

	return thread_task_post(tp ,task);
}

std::vector<iEvent*> DispatchMsgService::process(const iEvent * ev)
{
	LOG_DEBUG("DispatchMsgService::process - ev %p\n", ev);
	if (ev == NULL)
	{
		return {};
	}

	u32 eid = ev->get_eid();

	LOG_DEBUG("DispatchMsgService::process - eid %u\n", eid);

	if (eid == EEVEBTID_UNKNOW)
	{
		LOG_WARN("DispatchMsgService::process unkonw evend id %d\n", eid);
		return {};
	}

	T_EventHandlersMap::iterator handlers = subscribers_.find(eid);
	if (handlers == subscribers_.end())
	{
		LOG_WARN("DispatchMsgService::process no any event handler subscribed %d\n", eid);
		return {};
	}

	std::vector<iEvent*> rsp_vector;
	for (auto iter = handlers->second.begin(); iter != handlers->second.end(); iter++)
	{
		iEventHandler* handler = *iter;
		LOG_DEBUG("DispatchMsgService::process get handler: %s\n", handler->get_name().c_str());
		rsp_vector.push_back(handler->handle(ev));
	}
	return rsp_vector;
}

void DispatchMsgService::svc(void * argv)
{
	DispatchMsgService* dms = DispatchMsgService::getInstance();
	iEvent* ev = (iEvent*)argv;
	if (!dms->svr_exit_)
	{
		LOG_DEBUG("DispatchMsgService::svc...\n");
		std::vector<iEvent*> rsp_vector = dms->process(ev);
		if (!rsp_vector.empty())
		{
			for (auto &it : rsp_vector)
			{
				it->dump(std::cout);
				it->set_args(ev->get_args());

				thread_mutex_lock(&queue_mutex);
				response_events.push(it);
				thread_mutex_unlock(&queue_mutex);
			}
		}
		else
		{
			//终止响应事件
			iEvent* rsp = new ExitRspEv();
			rsp->set_args(ev->get_args());

			thread_mutex_lock(&queue_mutex);
			response_events.push(rsp);
			thread_mutex_unlock(&queue_mutex);
		}
	}
}

DispatchMsgService * DispatchMsgService::getInstance()
{
	if (DMS_ == nullptr)
	{
		DMS_ = new DispatchMsgService();
	}
	return DMS_;
}

iEvent * DispatchMsgService::parseEvent(const char * message, u32 len, u32 eid)
{
	if (!message)
	{
		LOG_ERROR("DispatchMsgService::parseEvent - message is null[eid: %d].\n", eid);
		return nullptr;
	}
	if (eid == EEVENTID_GET_MOBILE_CODE_REQ)
	{
		tutorial::mobile_request mr;
		if (mr.ParseFromArray(message, len))
		{
			MobileCodeReqEv* ev = new MobileCodeReqEv(mr.mobile());
			return ev;
		}
	}
	else if (eid == EEVENTID_LOGIN_REQ)
	{
		tutorial::login_request lr;
		if (lr.ParseFromArray(message, len))
		{
			LoginReqEv* ev = new LoginReqEv(lr.mobile(), lr.icode());
			return ev;
		}
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
	return nullptr;
}

void DispatchMsgService::handleAllResponseEvent(NetworkInterface* interface)
{
	bool done = false;
	while (!done)
	{
		iEvent* ev = nullptr;
		thread_mutex_lock(&queue_mutex);
		if (!response_events.empty())
		{
			ev = response_events.front();
			response_events.pop();
		}
		else
		{
			done = true;
		}
		thread_mutex_unlock(&queue_mutex);

		if (!done)
		{
			if (ev->get_eid() == EEVENTID_GET_MOBILE_CODE_RSP)
			{
				MobileCodeRspEv* mcre = static_cast<MobileCodeRspEv*>(ev);
				LOG_DEBUG("DispatchMsgService::handleAllResponseEvent - id: EEVENTID_GET_MOBILE_CODE_RSP\n");

				ConnectSession* cs = (ConnectSession*)ev->get_args();
				cs->response = ev;

				cs->message_len = mcre->ByteSize();
				cs->write_buf = new char[cs->message_len + MESSAGE_HEADER_LEN];

				//组装头部
				memcpy(cs->write_buf, MESSAGE_HEADER_ID, strlen(MESSAGE_HEADER_ID));
				*(u16*)(cs->write_buf + 4) = EEVENTID_GET_MOBILE_CODE_RSP;
				*(i32*)(cs->write_buf + 6) = cs->message_len;

				mcre->SerializeToArray(cs->write_buf + MESSAGE_HEADER_LEN, cs->message_len);

				interface->send_response_message(cs);
			}
			else if(ev->get_eid() == EEVEBTID_EXIT_RSP)
			{
				ConnectSession* cs = (ConnectSession*)ev->get_args();
				cs->response = ev;
				interface->send_response_message(cs);
			}
		}
	}
}