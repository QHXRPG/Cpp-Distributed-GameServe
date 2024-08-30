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
这个实现文件提供了 Robot 类的具体实现，
包括从对象池中唤醒、更新、获取账户、发送账户检查消息、注册状态、处理账户检查响应和处理玩家列表等功能。
AwakeFromPool 初始化机器人状态管理器，添加消息组件和更新组件，从 YAML 文件中读取登录配置，并连接到服务器。
Update 方法更新网络连接和机器人状态。
GetAccount 返回机器人的账户。
RegisterState 注册机器人的各种状态。
HandleAccountCheckRs 处理账户检查响应，如果成功则切换到已登录状态，否则打印错误信息。
SendMsgAccountCheck 发送账户检查消息。
HandlePlayerList 处理玩家列表，如果为空则创建新玩家，否则切换到选择玩家状态。
*/

// 从对象池中唤醒机器人
void Robot::AwakeFromPool(std::string account) {
    _account = account;
    _isBroadcast = false;

    InitStateTemplateMgr(RobotStateType::RobotState_Login_Connecting);

    // 添加消息组件
    auto pMsgCallBack = new MessageCallBackFunctionFilterObj<Robot>();
    AddComponent<MessageComponent>(pMsgCallBack);
    pMsgCallBack->GetPacketObject = [this](SOCKET socket)->Robot* {
        if (this->GetSocket() == socket)
            return this;

        return nullptr;
    };
    pMsgCallBack->RegisterFunctionWithObj(Proto::MsgId::C2L_AccountCheckRs, BindFunP2(this, &Robot::HandleAccountCheckRs));
    pMsgCallBack->RegisterFunctionWithObj(Proto::MsgId::L2C_PlayerList, BindFunP2(this, &Robot::HandlePlayerList));

    // 添加更新组件
    auto pUpdateComponent = AddComponent<UpdateComponent>();
    pUpdateComponent->UpdataFunction = BindFunP0(this, &Robot::Update);

    // 从 YAML 文件中读取登录配置
    auto pYaml = Yaml::GetInstance();
    const auto pLoginConfig = dynamic_cast<LoginConfig*>(pYaml->GetConfig(APP_LOGIN));
    
    this->Connect(pLoginConfig->Ip, pLoginConfig->Port);
}

// 更新机器人状态
void Robot::Update() {
    NetworkConnector::Update();
    UpdateState();
}

// 获取机器人账户
std::string Robot::GetAccount() const {
    return _account;
}

// 注册机器人状态
void Robot::RegisterState() {
    RegisterStateClass(RobotStateType::RobotState_Login_Connecting, DynamicStateBind(RobotStateLoginConnectting));
    RegisterStateClass(RobotStateType::RobotState_Login_Connected, DynamicStateBind(RobotStateLoginConnectted));
    RegisterStateClass(RobotStateType::RobotState_Login_Logined, DynamicStateBind(RobotStateLoginLogined));
    RegisterStateClass(RobotStateType::RobotState_Login_SelectPlayer, DynamicStateBind(RobotStateLoginSelectPlayer));
}

// 处理账户检查响应
void Robot::HandleAccountCheckRs(Robot* pRobot, Packet* pPacket) {
    Proto::AccountCheckRs proto = pPacket->ParseToProto<Proto::AccountCheckRs>();
        //std::cout << "account check result account:" << _account << " code:" << proto.return_code() << std::endl;

    if (proto.return_code() == Proto::AccountCheckReturnCode::ARC_OK) {
        // 如果账户检查成功，切换到已登录状态
        ChangeState(RobotStateType::RobotState_Login_Logined);
    } else {
        // 否则打印错误信息
        std::cout << "account check failed. account:" << _account << " code:" << proto.return_code() << std::endl;
    }
}

// 发送账户检查消息
void Robot::SendMsgAccountCheck() {
    Proto::AccountCheck accountCheck;
    accountCheck.set_account(GetAccount());
    accountCheck.set_password("e10adc3949ba59abbe56e057f20f883e");

    auto pPacket = MessageSystemHelp::CreatePacket(Proto::MsgId::C2L_AccountCheck, GetSocket());
    pPacket->SerializeToBuffer(accountCheck);
    SendPacket(pPacket);
}

// 处理玩家列表
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
        // 如果玩家列表为空，则创建新玩家
        Proto::CreatePlayer protoCreate;

        // 随机生成一个角色名字后缀
        std::uniform_int_distribution<int> disName(1000, 10000 - 1);
        const int nNameRandom = disName(randomEngine);
        std::string playerName = strutil::format("%s-%d", _account.c_str(), nNameRandom);
        protoCreate.set_name(playerName.c_str());

        // 随机生成性别
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
        // 如果玩家列表不为空，切换到选择玩家状态
        //LOG_DEBUG("recv players. size:" << protoList.player_size());
        ChangeState(RobotStateType::RobotState_Login_SelectPlayer);
    }
}
