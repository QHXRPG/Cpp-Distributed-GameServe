#include "message_system.h"
#include "thread_mgr.h"
#include "network_locator.h"
#include "log4_help.h"

IMessageSystem::~IMessageSystem()
{
	if (_pCallBackFuns != nullptr)
	{
		delete _pCallBackFuns;
		_pCallBackFuns = nullptr;
	}
}

void IMessageSystem::AttachCallBackHandler(MessageCallBackFunctionInfo* pCallback)
{
	_pCallBackFuns = pCallback;
}

bool IMessageSystem::IsFollowMsgId(Packet* packet) const
{
	if (_pCallBackFuns == nullptr)
		return false;

	return _pCallBackFuns->IsFollowMsgId(packet);
}

void IMessageSystem::ProcessPacket(Packet* pPacket) const
{
	if (_pCallBackFuns == nullptr)
		return;

	_pCallBackFuns->ProcessPacket(pPacket);
}

void IMessageSystem::DispatchPacket(Packet* pPacket)
{
    ThreadMgr::GetInstance()->DispatchPacket(pPacket);
}

void IMessageSystem::DispatchPacket(const Proto::MsgId msgId, const SOCKET socket)
{
    const auto pPacket = CreatePacket(msgId, socket);
    DispatchPacket(pPacket);
}

void IMessageSystem::DispatchPacket(const Proto::MsgId msgId, const SOCKET socket, google::protobuf::Message& proto)
{
    const auto pPacket = CreatePacket(msgId, socket);
    pPacket->SerializeToBuffer(proto);
    DispatchPacket(pPacket);
}

void IMessageSystem::SendPacket(const Proto::MsgId msgId, const SOCKET socket, google::protobuf::Message& proto)
{
    const auto pPacket = CreatePacket(msgId, socket);
    pPacket->SerializeToBuffer(proto);
    SendPacket(pPacket);
}

void IMessageSystem::SendPacket(const Proto::MsgId msgId, google::protobuf::Message& proto, APP_TYPE appType, int appId)
{
    auto packet = CreatePacket(msgId, 0);
    packet->SerializeToBuffer(proto);
    SendPacket(packet, appType, appId);
}

void IMessageSystem::SendPacket(Packet* packet, APP_TYPE appType, int appId)
{
    auto pNetworkLocator = ThreadMgr::GetInstance()->GetComponent<NetworkLocator>();
    auto pNetwork = pNetworkLocator->GetNetworkConnector(appType, appId);
    if (pNetwork != nullptr)
    {
        packet->SetSocket(pNetwork->GetSocket());
        pNetwork->SendPacket(packet);
        return;
    }

    if ((Global::GetInstance()->GetCurAppType() & appType) != 0)
    {
        // �����ڵ�ǰ�����У�ֱ��ת��
        // ���� curapptype == all ��ʱ��
        DispatchPacket(packet);
    }
    else
    {
        LOG_ERROR("can't find network. appType:" << AppTypeMgr::GetInstance()->GetAppName(appType).c_str() << " appId:" << appId);
    }
}

void IMessageSystem::SendPacket(Packet* pPacket)
{
	auto pNetworkLocator = ThreadMgr::GetInstance()->GetComponent<NetworkLocator>();
	auto pNetwork = pNetworkLocator->GetNetworkConnector(pPacket->GetSocket());
	if (pNetwork != nullptr)
	{
		pNetwork->SendPacket(pPacket);
		return;
	}

	// all in one ʱ���Ҳ���Network�����������̷߳���Э��
	if (Global::GetInstance()->GetCurAppType() == APP_ALL && pPacket->GetSocket() == 0)
	{
		DispatchPacket(pPacket);
		return;
	}

	// ���������listen��������
	pNetwork = pNetworkLocator->GetListen(NetworkTcpListen);
	pNetwork->SendPacket(pPacket);
}

Packet* IMessageSystem::CreatePacket(Proto::MsgId msgId, SOCKET socket)
{
    auto pPacket = new Packet(msgId, socket);
    return pPacket;
}
