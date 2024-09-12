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
    void AwakeFromPool(std::string account) override;               // �Ӷ�����л��ѻ�����
    void Update() override;                                         // ���»�����״̬
    std::string GetAccount() const;                                 // ��ȡ�������˻�
    void SendMsgAccountCheck();                                     // �����˻������Ϣ

protected:
    void RegisterState() override;                              // ע�������״̬
    
private:
    void HandleAccountCheckRs(Robot* pRobot, Packet* pPacket);  // �����˻������Ӧ
    void HandlePlayerList(Robot* pRobot, Packet* pPacket);      // ��������б�

private:
    std::string _account; // �������˻�
};
