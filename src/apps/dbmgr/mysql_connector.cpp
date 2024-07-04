#include "mysql_connector.h"
#include "libserver/log4_help.h"
#include <thread>

#define MysqlPingTime 2 * 60

void MysqlConnector::AwakeFromPool()
{
    // ���������Ϣ����¼�������ü���ǰ�߳�ID
    LOG_DEBUG("MysqlConnector::Awake. id:" << std::this_thread::get_id());

    // ��ȡYaml����ʵ��
    auto pYaml = Yaml::GetInstance();
    // ��ȡ���ݿ��������
    auto pConfig = pYaml->GetConfig(APP_DB_MGR);
    // ��̬ת��Ϊ���ݿ������������
    auto pDbCfig = dynamic_cast<DBMgrConfig*>(pConfig);

    // ��ȡMySQL���ݿ�����
    _pDbCfg = pDbCfig->GetDBConfig(DBMgrConfig::DBTypeMysql);
    // ����Ƿ�ɹ���ȡ����
    if (_pDbCfg == nullptr)
    {
        // ���δ��ȡ�����ã���¼������־
        LOG_ERROR("Failed to get mysql config.");
        return;
    }

    // ���ӵ����ݿ�
    Connect();
}


void MysqlConnector::CheckPing()
{
    //LOG_DEBUG( "check mysql ping." );

    if (_isRunning)
    {
        mysql_ping(_pMysql);
        if (CheckMysqlError() != 0)
        {
            Disconnect();
        }
    }

    if (!_isRunning)
    {
        ReConnect();
    }
}

void MysqlConnector::BackToPool()
{
    Disconnect();
}

bool MysqlConnector::Connect()
{
    // ��ʼ�����ӣ����ʧ���򷵻�false
    if (!ConnectInit())
        return false;

    // ���������Ϣ����¼�������ӵ����ݿ�IP���˿ں��߳�ID
    LOG_DEBUG("MysqlConnector::Connect. " << _pDbCfg->Ip.c_str() << ":" << _pDbCfg->Port << " starting... id:" << std::this_thread::get_id());

    // �������ӵ�MySQL���ݿ�
    mysql_real_connect(
        _pMysql,                             // MySQL���Ӷ���
        _pDbCfg->Ip.c_str(),                 // ���ݿ�IP��ַ
        _pDbCfg->User.c_str(),               // ���ݿ��û���
        _pDbCfg->Password.c_str(),           // ���ݿ�����
        _pDbCfg->DatabaseName.c_str(),       // ���ݿ�����
        _pDbCfg->Port,                       // ���ݿ�˿�
        nullptr,                             // Ĭ��Unix�׽��֣�Windows��ΪNULL
        CLIENT_FOUND_ROWS                    // �ͻ��˱�־������ƥ���ж�����Ӱ���е�����
    );

    // ��������Ƿ��д���
    int mysqlerrno = CheckMysqlError();
    if (mysqlerrno > 0)
    {
        // ����д��󣬶Ͽ����Ӳ�����false
        Disconnect();
        return false;
    }

    // ����Ҫ�ֶ��ر��Զ��ύģʽ���ײ����START TRANSACTION֮���Զ�COMMIT
    // mysql_autocommit(mysql(), 0);

    // ����һ��ping���󵽷�������ȷ��������Ч
    mysql_ping(_pMysql);
    mysqlerrno = CheckMysqlError();
    if (mysqlerrno > 0)
    {
        // ����д��󣬶Ͽ����Ӳ�����false
        Disconnect();
        return false;
    }

    // ���������Ϣ����¼�ɹ����ӵ����ݿ�IP�Ͷ˿�
    LOG_DEBUG("MysqlConnector::Connect: successfully! addr:" << _pDbCfg->Ip.c_str() << ":" << _pDbCfg->Port);

    // ��ʼ��Ԥ�������
    InitStmts();

    // �������������
    _isRunning = true;

    // ����true��ʾ�ɹ�����
    return true;
}


void MysqlConnector::ReConnect()
{
    if (!ConnectInit())
        return;

    LOG_DEBUG("MysqlConnector::ReConnect. " << _pDbCfg->Ip.c_str() << ":" << _pDbCfg->Port << " starting...");

    if (nullptr == mysql_real_connect(_pMysql, _pDbCfg->Ip.c_str(), _pDbCfg->User.c_str(), _pDbCfg->Password.c_str(), _pDbCfg->DatabaseName.c_str(), _pDbCfg->Port, nullptr, CLIENT_FOUND_ROWS))
    {
        LOG_ERROR("MysqlConnector::ReConnect failed. " << _pDbCfg->Ip.c_str() << ":" << _pDbCfg->Port << " starting...");
        CheckMysqlError();
        return;
    }

    mysql_select_db(_pMysql, _pDbCfg->DatabaseName.c_str());
    if (CheckMysqlError() != 0)
    {
        Disconnect();
        return;
    }

    LOG_DEBUG("MysqlConnector::ReConnect: successfully! addr:" << _pDbCfg->Ip.c_str() << ":" << _pDbCfg->Port);

    InitStmts();

    _isRunning = true;
}

void MysqlConnector::Disconnect()
{
    CleanStmts();
    MysqlBase::Disconnect();
}

void MysqlConnector::InitStmts()
{
    // ����һ�� SQL ������䣬�����������ݿ����� DatabaseStmtKey::StmtCreate ����
    DatabaseStmt* stmt = CreateStmt("insert into player ( sn, account, name, savetime, createtime ) value ( ?, ?, ?, now(), now() )");
    _mapStmt.insert(std::make_pair(DatabaseStmtKey::StmtCreate, stmt));

    // ����һ�� SQL ������䣬�����������ݿ����� DatabaseStmtKey::StmtSave ����
    stmt = CreateStmt("update player set base=?, misc=?,savetime=now() where sn = ?");
    _mapStmt.insert(std::make_pair(DatabaseStmtKey::StmtSave, stmt));

    // ��¼��ʼ���ɹ�����־
    LOG_DEBUG("\tMysqlConnector::InitStmts successfully!");
}

void MysqlConnector::CleanStmts()
{
    // �رղ��������д洢�� _mapStmt �е����
    for (auto one : _mapStmt)
    {
        one.second->Close();
    }

    // ������ӳ��
    _mapStmt.clear();
}

DatabaseStmt* MysqlConnector::GetStmt(DatabaseStmtKey stmtKey)
{
    // �����ض��������
    const auto iter = _mapStmt.find(stmtKey);
    if (iter == _mapStmt.end())
        return nullptr; // ����Ҳ��������� nullptr

    return iter->second; // �����ҵ������
}

DatabaseStmt* MysqlConnector::CreateStmt(const char* sql) const
{
    int str_len = strlen(sql); // ���� SQL ���ĳ���
    DatabaseStmt* stmt = new DatabaseStmt(); // �����µ����ݿ�Ԥ����������
    int param_count = 0; // ��ʼ������������

    stmt->stmt = mysql_stmt_init(_pMysql); // ��ʼ�� MySQL Ԥ�������
    if (mysql_stmt_prepare(stmt->stmt, sql, str_len) != 0)
    {
        // ���׼��Ԥ�������ʧ�ܣ���¼������־������ nullptr
        LOG_ERROR("create_stmt error, " << LOG4CPLUS_C_STR_TO_TSTRING(mysql_stmt_error(stmt->stmt)));
        return nullptr;
    }

    // ͳ�� SQL Ԥ��������еĲ�������
    for (int i = 0; i < str_len; i++)
    {
        if ((sql[i] == '?') || (sql[i] == '@'))
            param_count++;
    }

    if (param_count > 0)
    {
        // ����в�������ʼ��������Ͱ󶨻�����
        stmt->bind = new MYSQL_BIND[param_count];
        memset(stmt->bind, 0, sizeof(MYSQL_BIND) * param_count);
        stmt->bind_buffer = new char[MAX_BIND_BUFFER];
    }
    else
    {
        // ���û�в�����������ͻ�������Ϊ nullptr
        stmt->bind = nullptr;
        stmt->bind_buffer = nullptr;
    }

    return stmt; // ���ش�����Ԥ����������
}

// ������İ������Ͱ󶨻���������
void MysqlConnector::ClearStmtParam(DatabaseStmt* stmt)
{
    // ������İ������Ͱ󶨻���������
    stmt->bind_index = 0;
    stmt->bind_buffer_index = 0;
}

bool MysqlConnector::ExecuteStmt(DatabaseStmt* stmt, my_ulonglong& affected_rows)
{
    MYSQL_STMT* st = stmt->stmt; // ��ȡ MySQL ���ָ��
    if (stmt->bind != nullptr)
        mysql_stmt_bind_param(stmt->stmt, stmt->bind); // �󶨲���

    if (mysql_stmt_execute(st) != 0)
    {
        // ���ִ��ʧ�ܣ���¼������־������ false
        LOG_ERROR("execute_stmt error, " << LOG4CPLUS_C_STR_TO_TSTRING(mysql_stmt_error(st)));
        return false;
    }

    // ��ȡ��Ӱ�������
    affected_rows = mysql_stmt_affected_rows(st);
    return true; // ִ�гɹ������� true
}

bool MysqlConnector::ExecuteStmt(DatabaseStmt* stmt)
{
    MYSQL_STMT* st = stmt->stmt; // ��ȡ MySQL ���ָ��
    if (stmt->bind != nullptr)
        mysql_stmt_bind_param(stmt->stmt, stmt->bind); // �󶨲���

    if (mysql_stmt_execute(st) != 0)
    {
        // ���ִ��ʧ�ܣ���¼������־������ false
        LOG_ERROR("execute_stmt error, " << LOG4CPLUS_C_STR_TO_TSTRING(mysql_stmt_error(st)));
        return false;
    }

    return true; // ִ�гɹ������� true
}


void MysqlConnector::AddParamBlob(DatabaseStmt* stmt, void* val, int size)
{
    MYSQL_BIND* pBind = &stmt->bind[stmt->bind_index];

    pBind->buffer_type = MYSQL_TYPE_BLOB;
    pBind->buffer = val;
    pBind->length = reinterpret_cast<unsigned long*>(&stmt->bind_buffer[stmt->bind_buffer_index + sizeof(void*)]);

    *(pBind->length) = size;
    pBind->buffer_length = size;

    stmt->bind_index++;
    stmt->bind_buffer_index += (sizeof(void*) + sizeof(unsigned long*));
}

void MysqlConnector::AddParamUint64(DatabaseStmt* stmt, uint64 val)
{
    MYSQL_BIND* pBind = &stmt->bind[stmt->bind_index]; // ��ȡ��ǰ��λ�õ�ָ��

    pBind->buffer_type = MYSQL_TYPE_LONGLONG; // ���ò�������Ϊ�޷��ų�����
    pBind->buffer = &stmt->bind_buffer[stmt->bind_buffer_index]; // �󶨻�����ָ��
    pBind->is_unsigned = true; // ���Ϊ�޷���

    *static_cast<uint64*>(pBind->buffer) = val; // ������ֵ���Ƶ�������

    stmt->bind_index++; // ���°�����
    stmt->bind_buffer_index += sizeof(uint64); // ���»���������
}

void MysqlConnector::AddParamInt(DatabaseStmt* stmt, int val)
{
    MYSQL_BIND* pBind = &stmt->bind[stmt->bind_index]; // ��ȡ��ǰ��λ�õ�ָ��

    pBind->buffer_type = MYSQL_TYPE_LONG; // ���ò�������Ϊ����
    pBind->buffer = &stmt->bind_buffer[stmt->bind_buffer_index]; // �󶨻�����ָ��
    pBind->is_unsigned = false; // ���Ϊ�з���

    *static_cast<long*>(pBind->buffer) = val; // ������ֵ���Ƶ�������

    stmt->bind_index++; // ���°�����
    stmt->bind_buffer_index += sizeof(long); // ���»���������
}

void MysqlConnector::AddParamStr(DatabaseStmt* stmt, const char* val)
{
    MYSQL_BIND* pBind = &stmt->bind[stmt->bind_index]; // ��ȡ��ǰ��λ�õ�ָ��
    int len = strlen(val); // ��ȡ�ַ�������
    if (len >= MAX_BIND_STR)
    {
        LOG_ERROR("add_str_param error, str size:" << len << " out of limit:" << MAX_BIND_STR); // ��ӡ������־
        len = MAX_BIND_STR - 1; // �����ַ�������
    }

    pBind->buffer_type = MYSQL_TYPE_STRING; // ���ò�������Ϊ�ַ���
    pBind->buffer = &stmt->bind_buffer[stmt->bind_buffer_index]; // �󶨻�����ָ��
    pBind->length = (unsigned long*)& stmt->bind_buffer[stmt->bind_buffer_index + len + 1]; // �����ַ�������ָ��

    engine_strncpy((char*)pBind->buffer, len + 1, val, len + 1); // �����ַ�����������
    *(pBind->length) = len; // �����ַ�������
    pBind->buffer_length = len; // ���û���������

    stmt->bind_index++; // ���°�����
    stmt->bind_buffer_index += (len + 1 + sizeof(unsigned long*)); // ���»���������
}
