#pragma once

#include "robot_state.h"

// 这个文件定义了四种具体的机器人状态类，
// 分别是 RobotStateLoginConnectting（连接中）、RobotStateLoginConnectted（已连接）、
// RobotStateLoginLogined（已登录）和 RobotStateLoginSelectPlayer（选择玩家）。
// 每个状态类继承自 RobotState，并实现了进入状态和更新状态的方法。通过宏 DynamicStateCreate，
// 每个状态类都可以动态创建实例并返回相应的状态类型。

// 定义机器人状态：连接中
class RobotStateLoginConnectting : public RobotState {
public:
    DynamicStateCreate(RobotStateLoginConnectting, RobotState_Login_Connecting);

    void OnEnterState() override; // 进入状态时的操作
    RobotStateType OnUpdate() override; // 更新状态
};

// 定义机器人状态：已连接
class RobotStateLoginConnectted : public RobotState {
public:
    DynamicStateCreate(RobotStateLoginConnectted, RobotState_Login_Connected);

    void OnEnterState() override; // 进入状态时的操作
};

// 定义机器人状态：已登录
class RobotStateLoginLogined : public RobotState {
public:
    DynamicStateCreate(RobotStateLoginLogined, RobotState_Login_Logined);
};

// 定义机器人状态：选择玩家
class RobotStateLoginSelectPlayer : public RobotState {
public:
    DynamicStateCreate(RobotStateLoginSelectPlayer, RobotState_Login_SelectPlayer);
};
