#include "robot_state_login.h"
#include "robot.h"

/*
����ļ�ʵ���� robot_state_login.h �ж���ľ���״̬��ķ�����RobotStateLoginConnectting ��ʵ���˽���״̬�͸���״̬�ķ�����
����״̬ʱ���Խ��г�ʼ������������״̬ʱ��������״̬�����Ƿ��л���������״̬��RobotStateLoginConnectted ��ʵ���˽���״̬�ķ�����
����״̬ʱ�����˻������Ϣ��
*/

// ������״̬�������У�����״̬ʱ�Ĳ���
void RobotStateLoginConnectting::OnEnterState() {
    // �˴�������ӽ���������״̬ʱ�ĳ�ʼ���߼�
}

// ������״̬�������У�����״̬
RobotStateType RobotStateLoginConnectting::OnUpdate() {
    // ���������Ƿ�������
    if (_pParentObj->IsConnected()) {
        // ��������ӣ��л���������״̬
        return RobotState_Login_Connected;
    }

    // ���򱣳ֵ�ǰ״̬
    return GetState();
}

// ������״̬�������ӣ�����״̬ʱ�Ĳ���
void RobotStateLoginConnectted::OnEnterState() {
    // �����˻������Ϣ
    _pParentObj->SendMsgAccountCheck();
}
