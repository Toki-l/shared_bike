#ifndef NS_EVENT_H_
#define	NS_EVENT_H_

#include "glo_def.h"
#include "eventtype.h"
#include <string>
#include <iostream>

//�¼�
class iEvent
{
public:
	iEvent(u32 eid, u32 sn);

	virtual ~iEvent();
	virtual std::ostream &dump(std::ostream& out) const { return out; };
	virtual i32 ByteSize() { return 0; };
	virtual bool SerializeToArray(char* buf, int len) { return true; };

	u32 generateSeqNo();	//�������к�

	u32 get_eid() const { return eid_; };
	u32 get_sn() const { return sn_; };

	void set_eid(u32 eid) { eid_ = eid; };
	void set_sn(u32 sn) { sn_ = sn; };

	void set_args(void* args) { args_ = args; };
	void* get_args() { return args_; };

private:
	u32 eid_;
	u32 sn_;	//���к�
	void* args_;
};


#endif // !NS_EVENT_H_

