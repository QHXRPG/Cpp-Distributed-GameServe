#include "test_msg_handler.h"
#include "libserver/common.h"
#include "libserver/protobuf/proto_id.pb.h"
#include "libserver/protobuf/msg.pb.h"
#include "libserver/packet.h"

bool TestMsgHandler::Init()
{
    return true;
}

// 继承自ThreadObject类的RegisterMsgFunction方法
void TestMsgHandler::RegisterMsgFunction()
{
    // 绑定一个由MI_TestMsg信号触发的函数HandleMsg()
    RegisterFunction((int)Proto::MsgId::MI_TestMsg, BindFunP1(this, &TestMsgHandler::HandleMsg));
}

void TestMsgHandler::Update()
{
    
}

void TestMsgHandler::HandleMsg(Packet* pPacket)
{
    auto protoObj = pPacket->ParseToProto<Proto::TestMsg>();
    std::cout << protoObj.msg().c_str() << std::endl;
}
