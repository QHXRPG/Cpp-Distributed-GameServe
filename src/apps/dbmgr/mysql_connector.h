#pragma once

#include "libserver/common.h"
#include "libserver/yaml.h"
#include "libserver/util_time.h"
#include "libserver/entity.h"
#include "libserver/system.h"
#include "libserver/message_system.h"
#include "mysql_base.h"

#include <mysql/mysql.h>
#include <vector>

class Packet;

enum DatabaseStmtKey
{
    StmtCreate,  // 创建数据库语句
    StmtSave,    // 保存数据库语句
};

struct DatabaseStmt
{
    MYSQL_STMT *stmt{ nullptr };      // MySQL预处理语句指针
    MYSQL_BIND *bind{ nullptr };      // MySQL绑定参数指针
    char *bind_buffer{ nullptr };     // 绑定缓冲区
    int bind_index;                   // 绑定索引
    int bind_buffer_index;            // 绑定缓冲区索引

    void Close() const                // 关闭并清理资源
    {
        if (bind != nullptr)
            delete[] bind;

        if (bind_buffer != nullptr)
            delete bind_buffer;

        if (stmt != nullptr)
            mysql_stmt_close(stmt);
    }
};

#define MAX_BIND_BUFFER    40960  // 最大绑定缓冲区大小
#define MAX_BIND_STR       30000  // 最大绑定字符串大小

class MysqlConnector : public MysqlBase, public Entity<MysqlConnector>, public IMessageSystem, public IAwakeFromPoolSystem<>
{
public:
    void AwakeFromPool() override;      // 从对象池中唤醒
    void BackToPool() override;         // 归还到对象池
    void RegisterMsgFunction() override;// 注册消息处理函数

    bool Connect();                     // 连接到数据库
    void Disconnect() override;         // 断开与数据库的连接

private:
    void ReConnect();                   // 重新连接数据库

    void CheckPing();                   // 检查与数据库的连接状态

#pragma region 预处理

    void InitStmts();                                // 初始化预处理语句
    void CleanStmts();                               // 清理预处理语句
    DatabaseStmt* GetStmt(DatabaseStmtKey stmtKey);  // 获取特定的预处理语句

    DatabaseStmt* CreateStmt(const char *sql) const;          // 创建新的预处理语句
    void ClearStmtParam(DatabaseStmt* stmt);                  // 清除预处理语句的参数
    static void AddParamStr(DatabaseStmt* stmt, const char* value);   // 添加字符串参数
    static void AddParamInt(DatabaseStmt* stmt, int val);             // 添加整数参数
    static void AddParamUint64(DatabaseStmt* stmt, uint64 val);       // 添加64位无符号整数参数
    static void AddParamBlob(DatabaseStmt* stmt, void *val, int size);// 添加二进制数据参数
    bool ExecuteStmt(DatabaseStmt* stmt, my_ulonglong& affected_rows);// 执行预处理语句并获取受影响的行数
    bool ExecuteStmt(DatabaseStmt* stmt);                             // 执行预处理语句

#pragma endregion 

    // 协议处理
    void HandleQueryPlayerList(Packet* pPacket);           // 处理查询玩家列表的请求
    void QueryPlayerList(std::string account, SOCKET socket);// 查询玩家列表

    void HandleQueryPlayer(Packet* pPacket);               // 处理查询玩家的请求

    void HandleCreatePlayer(Packet* pPacket);              // 处理创建玩家的请求
    void HandleSavePlayer(Packet* pPacket);                // 处理保存玩家的请求
    bool OnSavePlayer(DatabaseStmt* stmtSave, Proto::Player& protoPlayer); // 保存玩家数据

protected:
    // 预处理语句映射
    std::map<DatabaseStmtKey, DatabaseStmt*> _mapStmt;

    bool _isRunning{ false };     // 是否在运行状态
};

