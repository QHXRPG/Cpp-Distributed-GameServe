#include "robot.h"

#include "robot_state_login.h"

#include "libserver/common.h"
#include "libserver/packet.h"
#include "libserver/robot_state_type.h"
#include "libserver/yaml.h"
#include "libserver/entity_system.h"
#include "libserver/log4_help.h"
#include "libserver/message_system_help.h"
#include "libserver/message_component.h"
#include "libserver/update_component.h"

#include <random>
#include <thread>



/*
���ʵ���ļ��ṩ�� Robot ��ľ���ʵ�֣�
�����Ӷ�����л��ѡ����¡���ȡ�˻��������˻������Ϣ��ע��״̬�������˻������Ӧ�ʹ�������б�ȹ��ܡ�
AwakeFromPool ��ʼ��������״̬�������������Ϣ����͸���������� YAML �ļ��ж�ȡ��¼���ã������ӵ���������
Update ���������������Ӻͻ�����״̬��
GetAccount ���ػ����˵��˻���
RegisterState ע������˵ĸ���״̬��
HandleAccountCheckRs �����˻������Ӧ������ɹ����л����ѵ�¼״̬�������ӡ������Ϣ��
SendMsgAccountCheck �����˻������Ϣ��
HandlePlayerList ��������б����Ϊ���򴴽�����ң������л���ѡ�����״̬��
*/

// �Ӷ�����л��ѻ�����
void Robot::AwakeFromPool(std::string account) {
    _account = account;
    _isBroadcast = false;

    InitStateTemplateMgr(RobotStateType::RobotState_Login_Connecting);

    // �����Ϣ���
    auto pMsgCallBack = new MessageCallBackFunctionFilterObj<Robot>();
    AddComponent<MessageComponent>(pMsgCallBack);
    pMsgCallBack->GetPacketObject = [this](SOCKET socket)->Robot* {
        if (this->GetSocket() == socket)
            return this;

        return nullptr;
    };
    pMsgCallBack->RegisterFunctionWithObj(Proto::MsgId::C2L_AccountCheckRs, BindFunP2(this, &Robot::HandleAccountCheckRs));
    pMsgCallBack->RegisterFunctionWithObj(Proto::MsgId::L2C_PlayerList, BindFunP2(this, &Robot::HandlePlayerList));

    // ��Ӹ������
    auto pUpdateComponent = AddComponent<UpdateComponent>();
    pUpdateComponent->UpdataFunction = BindFunP0(this, &Robot::Update);

    // �� YAML �ļ��ж�ȡ��¼����
    auto pYaml = Yaml::GetInstance();
    const auto pLoginConfig = dynamic_cast<LoginConfig*>(pYaml->GetConfig(APP_LOGIN));
    
    this->Connect(pLoginConfig->Ip, pLoginConfig->Port);
}

// ���»�����״̬
void Robot::Update() {
    NetworkConnector::Update();
    UpdateState();
}

// ��ȡ�������˻�
std::string Robot::GetAccount() const {
    return _account;
}

// ע�������״̬
void Robot::RegisterState() {
    RegisterStateClass(RobotStateType::RobotState_Login_Connecting, DynamicStateBind(RobotStateLoginConnectting));
    RegisterStateClass(RobotStateType::RobotState_Login_Connected, DynamicStateBind(RobotStateLoginConnectted));
    RegisterStateClass(RobotStateType::RobotState_Login_Logined, DynamicStateBind(RobotStateLoginLogined));
    RegisterStateClass(RobotStateType::RobotState_Login_SelectPlayer, DynamicStateBind(RobotStateLoginSelectPlayer));
}

// �����˻������Ӧ
void Robot::HandleAccountCheckRs(Robot* pRobot, Packet* pPacket) {
    Proto::AccountCheckRs proto = pPacket->ParseToProto<Proto::AccountCheckRs>();
        //std::cout << "account check result account:" << _account << " code:" << proto.return_code() << std::endl;

    if (proto.return_code() == Proto::AccountCheckReturnCode::ARC_OK) {
        // ����˻����ɹ����л����ѵ�¼״̬
        ChangeState(RobotStateType::RobotState_Login_Logined);
    } else {
        // �����ӡ������Ϣ
        std::cout << "account check failed. account:" << _account << " code:" << proto.return_code() << std::endl;
    }
}

// �����˻������Ϣ
void Robot::SendMsgAccountCheck() {
    Proto::AccountCheck accountCheck;
    accountCheck.set_account(GetAccount());
    accountCheck.set_password("e10adc3949ba59abbe56e057f20f883e");

    auto pPacket = MessageSystemHelp::CreatePacket(Proto::MsgId::C2L_AccountCheck, GetSocket());
    pPacket->SerializeToBuffer(accountCheck);
    SendPacket(pPacket);
}

// ��������б�
void Robot::HandlePlayerList(Robot* pRobot, Packet* pPacket) {
    auto protoList = pPacket->ParseToProto<Proto::PlayerList>();
    //LOG_DEBUG("player list. account:" << GetAccount().c_str() << " size:" << protoList.player_size());

    std::stringstream strStream;
    strStream << std::this_thread::get_id();
    std::string idstr = strStream.str();
    std::seed_seq seed1(idstr.begin(), idstr.end());
    std::minstd_rand0 generator(seed1);
    std::default_random_engine randomEngine(generator());

    if (protoList.player_size() == 0) {
        // �������б�Ϊ�գ��򴴽������
        Proto::CreatePlayer protoCreate;

        // �������һ����ɫ���ֺ�׺
        std::uniform_int_distribution<int> disName(1000, 10000 - 1);
        const int nNameRandom = disName(randomEngine);
        std::string playerName = strutil::format("%s-%d", _account.c_str(), nNameRandom);
        protoCreate.set_name(playerName.c_str());

        // ��������Ա�
        std::uniform_int_distribution<int> disGender(0, 1);
        const int nGender = disGender(randomEngine);
        if (nGender == 1)
            protoCreate.set_gender(Proto::Gender::male);
        else
            protoCreate.set_gender(Proto::Gender::female);

        //LOG_DEBUG("create. name:" << playerName.c_str() << " gender:" << (int)Proto::Gender::male);

        Packet* pPacketCreate = MessageSystemHelp::CreatePacket(Proto::MsgId::C2L_CreatePlayer, GetSocket());
        pPacketCreate->SerializeToBuffer(protoCreate);
        SendPacket(pPacketCreate);
    } else {
        // �������б�Ϊ�գ��л���ѡ�����״̬
        //LOG_DEBUG("recv players. size:" << protoList.player_size());
        ChangeState(RobotStateType::RobotState_Login_SelectPlayer);
    }
}
