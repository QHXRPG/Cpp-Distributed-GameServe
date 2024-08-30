#include "robot_state.h"
#include "libserver/packet.h"
#include "libserver/network_connector.h"
#include "robot.h"
#include "libserver/thread_mgr.h"

/*
这个实现文件提供了 RobotState 类的具体实现。Update 方法检查机器人是否断线，
如果断线则切换到 Login_Connecting 状态。EnterState 方法在每次进入状态时通知主线程，
并调用子类的 OnEnterState 方法。LeaveState 方法调用子类的 OnLeaveState 方法。
*/

// 检测是否已断线
RobotStateType RobotState::Update() {
    const auto state = GetState();
    // 如果当前状态大于 Login_Connecting 且不等于 Game_Connecting，检查连接状态
    if (state > RobotState_Login_Connecting && state != RobotState_Game_Connecting) {
        if (!_pParentObj->IsConnected()) {
            // 如果未连接，切换到 Login_Connecting 状态
            return RobotState_Login_Connecting;
        }
    }

    return OnUpdate();
}

// 进入状态
void RobotState::EnterState() {
    // 每进入一个状态，通知 robot mgr
    Proto::RobotSyncState protoState;
    auto pState = protoState.add_states();
    pState->set_account(_pParentObj->GetAccount());
    pState->set_state(GetState());

    // 只发送给主线程
    auto pPacket = MessageSystemHelp::CreatePacket(Proto::MsgId::MI_RobotSyncState, 0);
    pPacket->SerializeToBuffer(protoState);
    ThreadMgr::GetInstance()->GetMessageSystem()->AddPacketToList(pPacket);

    OnEnterState();
}

// 离开状态
void RobotState::LeaveState() {
    OnLeaveState();
}
