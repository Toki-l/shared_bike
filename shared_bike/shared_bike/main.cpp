#include <iostream>
#include <unistd.h>
#include "bike.pb.h"
#include "event.h"
#include "event_def.h"
#include "user_event_handler.h"
#include "DispatchMsgService.h"
#include "NetworkInterface.h"
#include "iniconfig.h"
#include "configdef.h"
#include "Logger.h"
#include "sqlconnection.h"
#include "BusProcessor.h"

using namespace std;

int main(int argc, char**argv)
{
	if (argc != 3)
	{
		printf("Please input shbk <config file path> <log file config>!\n");
		return -1;
	}

	if (!Logger::instance()->init(std::string(argv[2])))
	{
		fprintf(stderr, "init log module failed.\n");
		return -2;
	}

	Iniconfig config;
	if (!config.loadfile(std::string(argv[1])))
	{
		LOG_ERROR("load %s failed.\n", argv[1]);
		return -3;
	}
	st_env_config conf_args = config.getconfig();

	std::shared_ptr<MysqlConnection> conn(new MysqlConnection);
	conn->Init(conf_args.db_ip.c_str(), conf_args.db_port, conf_args.db_user.c_str(), conf_args.db_pwd.c_str(), conf_args.db_name.c_str());
	BusinessProcessor *bus = new BusinessProcessor(conn);
	bus->init();
	//tutorial::mobile_request msg;
	//msg.set_mobile("1009621693");

	//cout << msg.mobile() << endl;

	//iEvent* ievent = new iEvent(EEVENTID_GET_MOBILE_CODE_REQ, 2);

	//MobileCodeReqEv me(msg.mobile());
	//me.dump(cout);

	//MobileCodeRspEv mr(200, 123456);
	////mr.dump(cout);

	//UserEventHandler* userEventHandler = new UserEventHandler();
	//userEventHandler->handle(&me);

	DispatchMsgService * dms = DispatchMsgService::getInstance();
	dms->open();
	MobileCodeReqEv *mcre = new MobileCodeReqEv("12345asdasd");

	dms->enqueue(mcre);
	dms->close();
	NetworkInterface* NTIF = new NetworkInterface();
	NTIF->start(8888);

	//MysqlConnection mysqlConn;
	

	while (1)
	{
		NTIF->network_event_dispatch();
		sleep(1);
		LOG_DEBUG("network_event_dispatch...\n");
	}

	return 0;
}