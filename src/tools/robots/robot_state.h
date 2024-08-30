#pragma once

#include "libserver/state_template.h"
#include "libserver/robot_state_type.h"

class Robot;

/*
���ͷ�ļ�������һ��������״̬���� RobotState���̳��� StateTemplate��
�����ṩ�˸���״̬������״̬���뿪״̬�ķ�����
�������������Щ�麯����ʵ�־����״̬�߼���
*/

// ���������״̬����
class RobotState : public StateTemplate<RobotStateType, Robot> {
public:
    // ����״̬
    RobotStateType Update() override;

    // �麯��������������أ�ʵ�־���״̬�ĸ����߼�
    virtual RobotStateType OnUpdate() {
        return GetState();
    }

    // ����״̬
    void EnterState() override;

    // �麯��������������أ�ʵ�־���״̬�Ľ����߼�
    virtual void OnEnterState() {
    }

    // �뿪״̬
    void LeaveState() override;

    // �麯��������������أ�ʵ�־���״̬���뿪�߼�
    virtual void OnLeaveState() {
    }
};
