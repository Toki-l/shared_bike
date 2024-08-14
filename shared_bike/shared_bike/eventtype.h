#ifndef BRKS_COMMON_EVENT_TYPE_H
#define BRKS_COMMON_EVENT_TYPE_H

#include "glo_def.h"

//错误原因结构体
typedef struct EErrorReason_
{
	i32  code;
	const char* reason;
}EErrorReason;

/*事件ID*/
enum EventID
{
	EEVENTID_GET_MOBILE_CODE_REQ	  = 0x01,	//短信获取
	EEVENTID_GET_MOBILE_CODE_RSP	  = 0x02,

	EEVENTID_LOGIN_REQ				  = 0x03,	//登入验证
	EEVENTID_LOGIN_RSP				  = 0x04,

	EEVENTID_RECHARGE_REQ			  = 0x05,	//充值
	EEVENTID_RECHARGE_RSP			  = 0x06,
	
	EEVENTID_GET_ACCOUNT_BALANCE_REQ  = 0x07,	//余额查询
	EEVENTID_ACCOUNT_BALANCE_RSP	  = 0x08,	

	EEVEBTID_LIST_ACCOUNT_RECORDS_REQ = 0x09,	//查询充值记录
	EEVEBTID_ACCOUNT_RECORDS_RSP	  = 0x10,

	EEVEBTID_LIST_TRAVEL_REQ		  = 0x11,	//查询骑行记录
	EEVEBTID_LIST_TRAVEL_RSP		  = 0x12,

	EEVEBTID_EXIT_RSP				  = 0xFE,	//结束响应
	EEVEBTID_UNKNOW					  = 0xFF,	//未知事件
};

/*事件响应错误代号*/
enum EErroeCode
{
	ERRC_SUCCESS			= 200,
	ERRC_INVALID_MSG		= 400,		//无效的消息
	ERRC_INVALID_DATA		= 404,		//无效的数据
	ERRC_METHOD_NOT_ALLOWED = 405,		//方法不允许                   
	ERRO_PROCCESS_FAILED	= 406,		//程序错误
	ERRO_BIKE_IS_TOOK		= 407,		//单车丢失
	ERRO_BIKE_IS_RUNNING	= 408,		//单车正在骑行
	ERRO_BIKE_IS_DAMAGED	= 409,		//单车损坏
	ERR_NULL				= 0
};

//获取错误原因，根据错误代号code获取
const char* getReasonByErrorCode(i32 code);

#endif // !BRKS_COMMON_EVENT_TYPE_H

