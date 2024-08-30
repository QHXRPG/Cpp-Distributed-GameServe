#pragma once

#include "libserver/network_connector.h"
#include "libserver/state_template.h"
#include "libserver/robot_state_type.h"

#include "robot_state.h"

/*
���ͷ�ļ������� Robot �࣬�̳��� NetworkConnector �� StateTemplateMgr��
���ڹ�������˵�״̬���������ӡ��ṩ�˻��ѡ����¡���ȡ�˻��������˻������Ϣ��ע��״̬�ȹ��ܡ�
*/

// Robot ��̳��� NetworkConnector �� StateTemplateMgr�����ڹ�������˵�״̬����������
class Robot : public NetworkConnector, public StateTemplateMgr<RobotStateType, RobotState, Robot>, public IAwakeFromPoolSystem<std::string> {
public:
    // �Ӷ�����л��ѻ�����
    void AwakeFromPool(std::string account) override;
    // ���»�����״̬
    void Update() override;

    // ��ȡ�������˻�
    std::string GetAccount() const;
    // �����˻������Ϣ
    void SendMsgAccountCheck();

protected:
    // ע�������״̬
    void RegisterState() override;

private:
    // �����˻������Ӧ
    void HandleAccountCheckRs(Robot* pRobot, Packet* pPacket);
    // ��������б�
    void HandlePlayerList(Robot* pRobot, Packet* pPacket);

private:
    std::string _account; // �������˻�
};
