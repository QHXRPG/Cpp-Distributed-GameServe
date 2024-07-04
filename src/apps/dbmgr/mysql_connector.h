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
    StmtCreate,  // �������ݿ����
    StmtSave,    // �������ݿ����
};

struct DatabaseStmt
{
    MYSQL_STMT *stmt{ nullptr };      // MySQLԤ�������ָ��
    MYSQL_BIND *bind{ nullptr };      // MySQL�󶨲���ָ��
    char *bind_buffer{ nullptr };     // �󶨻�����
    int bind_index;                   // ������
    int bind_buffer_index;            // �󶨻���������

    void Close() const                // �رղ�������Դ
    {
        if (bind != nullptr)
            delete[] bind;

        if (bind_buffer != nullptr)
            delete bind_buffer;

        if (stmt != nullptr)
            mysql_stmt_close(stmt);
    }
};

#define MAX_BIND_BUFFER    40960  // ���󶨻�������С
#define MAX_BIND_STR       30000  // �����ַ�����С

class MysqlConnector : public MysqlBase, public Entity<MysqlConnector>, public IMessageSystem, public IAwakeFromPoolSystem<>
{
public:
    void AwakeFromPool() override;      // �Ӷ�����л���
    void BackToPool() override;         // �黹�������
    void RegisterMsgFunction() override;// ע����Ϣ������

    bool Connect();                     // ���ӵ����ݿ�
    void Disconnect() override;         // �Ͽ������ݿ������

private:
    void ReConnect();                   // �����������ݿ�

    void CheckPing();                   // ��������ݿ������״̬

#pragma region Ԥ����

    void InitStmts();                                // ��ʼ��Ԥ�������
    void CleanStmts();                               // ����Ԥ�������
    DatabaseStmt* GetStmt(DatabaseStmtKey stmtKey);  // ��ȡ�ض���Ԥ�������

    DatabaseStmt* CreateStmt(const char *sql) const;          // �����µ�Ԥ�������
    void ClearStmtParam(DatabaseStmt* stmt);                  // ���Ԥ�������Ĳ���
    static void AddParamStr(DatabaseStmt* stmt, const char* value);   // ����ַ�������
    static void AddParamInt(DatabaseStmt* stmt, int val);             // �����������
    static void AddParamUint64(DatabaseStmt* stmt, uint64 val);       // ���64λ�޷�����������
    static void AddParamBlob(DatabaseStmt* stmt, void *val, int size);// ��Ӷ��������ݲ���
    bool ExecuteStmt(DatabaseStmt* stmt, my_ulonglong& affected_rows);// ִ��Ԥ������䲢��ȡ��Ӱ�������
    bool ExecuteStmt(DatabaseStmt* stmt);                             // ִ��Ԥ�������

#pragma endregion 

    // Э�鴦��
    void HandleQueryPlayerList(Packet* pPacket);           // �����ѯ����б������
    void QueryPlayerList(std::string account, SOCKET socket);// ��ѯ����б�

    void HandleQueryPlayer(Packet* pPacket);               // �����ѯ��ҵ�����

    void HandleCreatePlayer(Packet* pPacket);              // ��������ҵ�����
    void HandleSavePlayer(Packet* pPacket);                // ��������ҵ�����
    bool OnSavePlayer(DatabaseStmt* stmtSave, Proto::Player& protoPlayer); // �����������

protected:
    // Ԥ�������ӳ��
    std::map<DatabaseStmtKey, DatabaseStmt*> _mapStmt;

    bool _isRunning{ false };     // �Ƿ�������״̬
};

