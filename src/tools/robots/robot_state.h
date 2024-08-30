#pragma once

#include "libserver/state_template.h"
#include "libserver/robot_state_type.h"

class Robot;

/*
这个头文件定义了一个机器人状态基类 RobotState，继承自 StateTemplate。
该类提供了更新状态、进入状态和离开状态的方法，
子类可以重载这些虚函数以实现具体的状态逻辑。
*/

// 定义机器人状态基类
class RobotState : public StateTemplate<RobotStateType, Robot> {
public:
    // 更新状态
    RobotStateType Update() override;

    // 虚函数，子类可以重载，实现具体状态的更新逻辑
    virtual RobotStateType OnUpdate() {
        return GetState();
    }

    // 进入状态
    void EnterState() override;

    // 虚函数，子类可以重载，实现具体状态的进入逻辑
    virtual void OnEnterState() {
    }

    // 离开状态
    void LeaveState() override;

    // 虚函数，子类可以重载，实现具体状态的离开逻辑
    virtual void OnLeaveState() {
    }
};
