#include "message_system.h"
#include "thread_mgr.h"
#include "network_locator.h"

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
