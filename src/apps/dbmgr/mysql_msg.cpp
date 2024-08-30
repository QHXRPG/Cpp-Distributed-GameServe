﻿
#include "mysql_connector.h"
#include "libserver/log4_help.h"
#include "libserver/message_component.h"
#include "libserver/message_system_help.h"

/* 
该实现文件定义了 MysqlConnector 类的方法，
负责初始化消息组件、处理不同协议消息以及执行相关的数据库操作。
主要实现了对玩家数据的查询、创建和保存功能。
*/

// 初始化消息回调函数并注册处理函数。
void MysqlConnector::InitMessageComponent()
{
    auto pMsgCallBack = new MessageCallBackFunction();
    AddComponent<MessageComponent>(pMsgCallBack);

    pMsgCallBack->RegisterFunction(Proto::MsgId::L2DB_QueryPlayerList, BindFunP1(this, &MysqlConnector::HandleQueryPlayerList));
    pMsgCallBack->RegisterFunction(Proto::MsgId::L2DB_CreatePlayer, BindFunP1(this, &MysqlConnector::HandleCreatePlayer));

    pMsgCallBack->RegisterFunction(Proto::MsgId::G2DB_SavePlayer, BindFunP1(this, &MysqlConnector::HandleSavePlayer));
    pMsgCallBack->RegisterFunction(Proto::MsgId::G2DB_QueryPlayer, BindFunP1(this, &MysqlConnector::HandleQueryPlayer));
}

void MysqlConnector::HandleQueryPlayerList(Packet* pPacket)
{
    auto protoQuery = pPacket->ParseToProto<Proto::QueryPlayerList>();
    QueryPlayerList(protoQuery.account(), pPacket->GetSocket());
}

// 处理查询玩家列表的请求。
void MysqlConnector::QueryPlayerList(std::string account, SOCKET socket)
{
    my_ulonglong affected_rows;  // 存储 MySQL 查询操作影响的行数
    // 构建查询语句，查询指定账号的玩家列表
    std::string sql = strutil::format("select sn, name, base, item, misc from player where account = '%s'", account.c_str());
    std::cout << "query:" << sql << std::endl;
    // 执行查询
    if (!Query(sql.c_str(), affected_rows))
    {
        LOG_ERROR("!!! Failed. MysqlConnector::HandleQueryPlayerList. sql:" << sql.c_str());
        return;
    }

    // 构建Proto消息，用于存储查询结果
    Proto::PlayerList protoRs;
    protoRs.set_account(account.c_str());

    Proto::PlayerBase protoBase;
    // 如果查询结果中有数据
    if (affected_rows > 0)
    {
        std::string tempStr;
        MYSQL_ROW row;   // 用于存储从 MySQL 查询结果中获取的一行数据
        
        // 遍历查询结果
        while ((row = Fetch()))
        {
            // 添加玩家信息到Proto消息中
            auto pProtoPlayer = protoRs.add_player();
            pProtoPlayer->set_sn(GetUint64(row, 0)); // 设置玩家唯一标识
            pProtoPlayer->set_name(GetString(row, 1)); // 设置玩家名字

            // 获取玩家基础信息Blob字段并解析
            GetBlob(row, 2, tempStr);
            protoBase.ParseFromString(tempStr);
            pProtoPlayer->set_level(protoBase.level()); // 设置玩家等级
            pProtoPlayer->set_gender(protoBase.gender()); // 设置玩家性别
        }
    }

    //LOG_DEBUG("player list. account:" << account.c_str() << " player list size:" << protoRs.player_size() << " socket:" << socket);

    // 无论是否找到玩家，都需要返回查询结果
    MessageSystemHelp::SendPacket(Proto::MsgId::L2DB_QueryPlayerListRs, socket, protoRs);
}


void MysqlConnector::HandleQueryPlayer(Packet* pPacket)
{
    auto protoQuery = pPacket->ParseToProto<Proto::QueryPlayer>();

    my_ulonglong affected_rows;
    std::string sql = strutil::format("select sn, name, account, base, item, misc from player where sn = %llu", protoQuery.player_sn());
    if (!Query(sql.c_str(), affected_rows))
    {
        LOG_ERROR("!!! Failed. MysqlConnector::HandleQueryPlayer. sql:" << sql.c_str());
        return;
    }

    Proto::QueryPlayerRs protoRs;
    if (affected_rows > 0)
    {
        std::string tempStr;
        MYSQL_ROW row;
        if ((row = Fetch()))
        {
            Proto::Player* pProtoPlayer = protoRs.mutable_player();
            pProtoPlayer->set_sn(GetUint64(row, 0));
            pProtoPlayer->set_name(GetString(row, 1));
            protoRs.set_account(GetString(row, 2));

            GetBlob(row, 3, tempStr);
            pProtoPlayer->mutable_base()->ParseFromString(tempStr);

            GetBlob(row, 5, tempStr);
            pProtoPlayer->mutable_misc()->ParseFromString(tempStr);
        }
    }

    //LOG_DEBUG("player  account:" << protoQuery.account().c_str() << " player list size:" << protoRs.player_size() << " socket:" << pPacket->GetSocket());
    MessageSystemHelp::SendPacket(Proto::MsgId::G2DB_QueryPlayerRs, pPacket->GetSocket(), protoRs);
}

void MysqlConnector::HandleCreatePlayer(Packet* pPacket)
{
    auto protoCreate = pPacket->ParseToProto<Proto::CreatePlayerToDB>();

    auto protoPlayer = protoCreate.player();

    // todo 是否有重名
    DatabaseStmt* stmt = GetStmt(DatabaseStmtKey::StmtCreate);
    if (stmt == nullptr)
        return;

    LOG_INFO("HandlePlayerCreate sn:" << protoPlayer.sn() << " account:" << protoCreate.account().c_str() << " name:" << protoPlayer.name().c_str());

    // create
    ClearStmtParam(stmt);
    AddParamUint64(stmt, protoPlayer.sn());
    AddParamStr(stmt, protoCreate.account().c_str());
    AddParamStr(stmt, protoPlayer.name().c_str());

    Proto::CreatePlayerToDBRs protoRs;
    protoRs.set_account(protoCreate.account().c_str());
    protoRs.set_return_code(Proto::CreatePlayerReturnCode::CPR_Unkonwn);

    if (ExecuteStmt(stmt))
    {
        protoRs.set_return_code(Proto::CreatePlayerReturnCode::CPR_Create_OK);
    }

    // 如果创建成功，将player list发送到客户端
    // 如果失败，将失败码返回到客户端
    if (protoRs.return_code() == Proto::CreatePlayerReturnCode::CPR_Create_OK)
    {
        QueryPlayerList(protoCreate.account(), pPacket->GetSocket());
    }
    else
    {
        MessageSystemHelp::SendPacket(Proto::MsgId::L2DB_CreatePlayerRs, pPacket->GetSocket(), protoRs);
    }
}

void MysqlConnector::HandleSavePlayer(Packet* pPacket)
{
    auto proto = pPacket->ParseToProto<Proto::SavePlayer>();
    DatabaseStmt* stmt = GetStmt(DatabaseStmtKey::StmtSave);
    if (stmt == nullptr)
        return;

    //LOG_INFO("HandleSavePlayer sn:" << proto.player_sn());
    Proto::Player protoPlayer = proto.player();
    OnSavePlayer(stmt, protoPlayer);
}

bool MysqlConnector::OnSavePlayer(DatabaseStmt* stmtSave, Proto::Player& protoPlayer)
{
    ClearStmtParam(stmtSave);

    std::string baseStr;
    protoPlayer.base().SerializeToString(&baseStr);
    AddParamBlob(stmtSave, (void*)baseStr.c_str(), (int)baseStr.size());

    std::string miscStr;
    protoPlayer.misc().SerializeToString(&miscStr);
    AddParamBlob(stmtSave, (void*)miscStr.c_str(), (int)miscStr.size());

    AddParamUint64(stmtSave, protoPlayer.sn());

    if (!ExecuteStmt(stmtSave))
    {
        LOG_ERROR("save player failed. player sn:" << protoPlayer.sn());
        return false;
    }

    return true;
}
