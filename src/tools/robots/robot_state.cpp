#include "robot_state.h"
#include "libserver/packet.h"
#include "libserver/network_connector.h"
#include "robot.h"
#include "libserver/thread_mgr.h"

// ����Ƿ��Ѷ���
RobotStateType RobotState::Update()
{
    const auto state = GetState();
    if (state > RobotState_Login_Connecting && state != RobotState_Game_Connecting)
    {
        if (!_pParentObj->IsConnected())
        {
            return RobotState_Login_Connecting;
        }
    }

    return OnUpdate();
}

void RobotState::EnterState()
{
    // ÿ����һ��״̬��֪ͨ robot mgr
    Proto::RobotSyncState protoState;
    auto pState = protoState.add_states();
    pState->set_account(_pParentObj->GetAccount());
    pState->set_state(GetState());

    // ֻ���͸����߳�
    auto pPacket = IMessageSystem::CreatePacket(Proto::MsgId::MI_RobotSyncState, 0);
    pPacket->SerializeToBuffer(protoState);
    ThreadMgr::GetInstance()->AddPacketToList(pPacket);

    OnEnterState();
}

void RobotState::LeaveState()
{
    OnLeaveState();
}
