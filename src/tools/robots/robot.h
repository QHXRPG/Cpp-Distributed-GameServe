#pragma once

#include "libserver/network_connector.h"
#include "libserver/state_template.h"
#include "libserver/robot_state_type.h"

#include "robot_state.h"

/*
这个头文件定义了 Robot 类，继承自 NetworkConnector 和 StateTemplateMgr，
用于管理机器人的状态和网络连接。提供了唤醒、更新、获取账户、发送账户检查消息、注册状态等功能。
*/

// Robot 类继承自 NetworkConnector 和 StateTemplateMgr，用于管理机器人的状态和网络连接
class Robot : public NetworkConnector, public StateTemplateMgr<RobotStateType, RobotState, Robot>, public IAwakeFromPoolSystem<std::string> {
public:
    // 从对象池中唤醒机器人
    void AwakeFromPool(std::string account) override;
    // 更新机器人状态
    void Update() override;

    // 获取机器人账户
    std::string GetAccount() const;
    // 发送账户检查消息
    void SendMsgAccountCheck();

protected:
    // 注册机器人状态
    void RegisterState() override;

private:
    // 处理账户检查响应
    void HandleAccountCheckRs(Robot* pRobot, Packet* pPacket);
    // 处理玩家列表
    void HandlePlayerList(Robot* pRobot, Packet* pPacket);

private:
    std::string _account; // 机器人账户
};
