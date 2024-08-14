#ifndef BRKS_COMMON_EVENT_TYPE_H
#define BRKS_COMMON_EVENT_TYPE_H

#include "glo_def.h"

//����ԭ��ṹ��
typedef struct EErrorReason_
{
	i32  code;
	const char* reason;
}EErrorReason;

/*�¼�ID*/
enum EventID
{
	EEVENTID_GET_MOBILE_CODE_REQ	  = 0x01,	//���Ż�ȡ
	EEVENTID_GET_MOBILE_CODE_RSP	  = 0x02,

	EEVENTID_LOGIN_REQ				  = 0x03,	//������֤
	EEVENTID_LOGIN_RSP				  = 0x04,

	EEVENTID_RECHARGE_REQ			  = 0x05,	//��ֵ
	EEVENTID_RECHARGE_RSP			  = 0x06,
	
	EEVENTID_GET_ACCOUNT_BALANCE_REQ  = 0x07,	//����ѯ
	EEVENTID_ACCOUNT_BALANCE_RSP	  = 0x08,	

	EEVEBTID_LIST_ACCOUNT_RECORDS_REQ = 0x09,	//��ѯ��ֵ��¼
	EEVEBTID_ACCOUNT_RECORDS_RSP	  = 0x10,

	EEVEBTID_LIST_TRAVEL_REQ		  = 0x11,	//��ѯ���м�¼
	EEVEBTID_LIST_TRAVEL_RSP		  = 0x12,

	EEVEBTID_EXIT_RSP				  = 0xFE,	//������Ӧ
	EEVEBTID_UNKNOW					  = 0xFF,	//δ֪�¼�
};

/*�¼���Ӧ�������*/
enum EErroeCode
{
	ERRC_SUCCESS			= 200,
	ERRC_INVALID_MSG		= 400,		//��Ч����Ϣ
	ERRC_INVALID_DATA		= 404,		//��Ч������
	ERRC_METHOD_NOT_ALLOWED = 405,		//����������                   
	ERRO_PROCCESS_FAILED	= 406,		//�������
	ERRO_BIKE_IS_TOOK		= 407,		//������ʧ
	ERRO_BIKE_IS_RUNNING	= 408,		//������������
	ERRO_BIKE_IS_DAMAGED	= 409,		//������
	ERR_NULL				= 0
};

//��ȡ����ԭ�򣬸��ݴ������code��ȡ
const char* getReasonByErrorCode(i32 code);

#endif // !BRKS_COMMON_EVENT_TYPE_H

