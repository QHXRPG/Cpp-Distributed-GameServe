#include "robot_state_login.h"
#include "robot.h"

/*
这个文件实现了 robot_state_login.h 中定义的具体状态类的方法。RobotStateLoginConnectting 类实现了进入状态和更新状态的方法，
进入状态时可以进行初始化操作，更新状态时根据连接状态决定是否切换到已连接状态。RobotStateLoginConnectted 类实现了进入状态的方法，
进入状态时发送账户检查消息。
*/

// 机器人状态：连接中，进入状态时的操作
void RobotStateLoginConnectting::OnEnterState() {
    // 此处可以添加进入连接中状态时的初始化逻辑
}

// 机器人状态：连接中，更新状态
RobotStateType RobotStateLoginConnectting::OnUpdate() {
    // 检查机器人是否已连接
    if (_pParentObj->IsConnected()) {
        // 如果已连接，切换到已连接状态
        return RobotState_Login_Connected;
    }

    // 否则保持当前状态
    return GetState();
}

// 机器人状态：已连接，进入状态时的操作
void RobotStateLoginConnectted::OnEnterState() {
    // 发送账户检查消息
    _pParentObj->SendMsgAccountCheck();
}
