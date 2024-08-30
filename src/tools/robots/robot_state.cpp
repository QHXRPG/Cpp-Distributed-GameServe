#include "robot_state.h"
#include "libserver/packet.h"
#include "libserver/network_connector.h"
#include "robot.h"
#include "libserver/thread_mgr.h"

/*
���ʵ���ļ��ṩ�� RobotState ��ľ���ʵ�֡�Update �������������Ƿ���ߣ�
����������л��� Login_Connecting ״̬��EnterState ������ÿ�ν���״̬ʱ֪ͨ���̣߳�
����������� OnEnterState ������LeaveState ������������� OnLeaveState ������
*/

// ����Ƿ��Ѷ���
RobotStateType RobotState::Update() {
    const auto state = GetState();
    // �����ǰ״̬���� Login_Connecting �Ҳ����� Game_Connecting���������״̬
    if (state > RobotState_Login_Connecting && state != RobotState_Game_Connecting) {
        if (!_pParentObj->IsConnected()) {
            // ���δ���ӣ��л��� Login_Connecting ״̬
            return RobotState_Login_Connecting;
        }
    }

    return OnUpdate();
}

// ����״̬
void RobotState::EnterState() {
    // ÿ����һ��״̬��֪ͨ robot mgr
    Proto::RobotSyncState protoState;
    auto pState = protoState.add_states();
    pState->set_account(_pParentObj->GetAccount());
    pState->set_state(GetState());

    // ֻ���͸����߳�
    auto pPacket = MessageSystemHelp::CreatePacket(Proto::MsgId::MI_RobotSyncState, 0);
    pPacket->SerializeToBuffer(protoState);
    ThreadMgr::GetInstance()->GetMessageSystem()->AddPacketToList(pPacket);

    OnEnterState();
}

// �뿪״̬
void RobotState::LeaveState() {
    OnLeaveState();
}
