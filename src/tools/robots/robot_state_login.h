#pragma once

#include "robot_state.h"

// ����ļ����������־���Ļ�����״̬�࣬
// �ֱ��� RobotStateLoginConnectting�������У���RobotStateLoginConnectted�������ӣ���
// RobotStateLoginLogined���ѵ�¼���� RobotStateLoginSelectPlayer��ѡ����ң���
// ÿ��״̬��̳��� RobotState����ʵ���˽���״̬�͸���״̬�ķ�����ͨ���� DynamicStateCreate��
// ÿ��״̬�඼���Զ�̬����ʵ����������Ӧ��״̬���͡�

// ���������״̬��������
class RobotStateLoginConnectting : public RobotState {
public:
    DynamicStateCreate(RobotStateLoginConnectting, RobotState_Login_Connecting);

    void OnEnterState() override; // ����״̬ʱ�Ĳ���
    RobotStateType OnUpdate() override; // ����״̬
};

// ���������״̬��������
class RobotStateLoginConnectted : public RobotState {
public:
    DynamicStateCreate(RobotStateLoginConnectted, RobotState_Login_Connected);

    void OnEnterState() override; // ����״̬ʱ�Ĳ���
};

// ���������״̬���ѵ�¼
class RobotStateLoginLogined : public RobotState {
public:
    DynamicStateCreate(RobotStateLoginLogined, RobotState_Login_Logined);
};

// ���������״̬��ѡ�����
class RobotStateLoginSelectPlayer : public RobotState {
public:
    DynamicStateCreate(RobotStateLoginSelectPlayer, RobotState_Login_SelectPlayer);
};
