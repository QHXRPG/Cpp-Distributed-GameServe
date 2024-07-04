#include "mysql_connector.h"
#include "libserver/log4_help.h"
#include <thread>

#define MysqlPingTime 2 * 60

void MysqlConnector::AwakeFromPool()
{
    // 输出调试信息，记录函数调用及当前线程ID
    LOG_DEBUG("MysqlConnector::Awake. id:" << std::this_thread::get_id());

    // 获取Yaml配置实例
    auto pYaml = Yaml::GetInstance();
    // 获取数据库管理配置
    auto pConfig = pYaml->GetConfig(APP_DB_MGR);
    // 动态转换为数据库管理配置类型
    auto pDbCfig = dynamic_cast<DBMgrConfig*>(pConfig);

    // 获取MySQL数据库配置
    _pDbCfg = pDbCfig->GetDBConfig(DBMgrConfig::DBTypeMysql);
    // 检查是否成功获取配置
    if (_pDbCfg == nullptr)
    {
        // 如果未获取到配置，记录错误日志
        LOG_ERROR("Failed to get mysql config.");
        return;
    }

    // 连接到数据库
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
    // 初始化连接，如果失败则返回false
    if (!ConnectInit())
        return false;

    // 输出调试信息，记录尝试连接的数据库IP、端口和线程ID
    LOG_DEBUG("MysqlConnector::Connect. " << _pDbCfg->Ip.c_str() << ":" << _pDbCfg->Port << " starting... id:" << std::this_thread::get_id());

    // 尝试连接到MySQL数据库
    mysql_real_connect(
        _pMysql,                             // MySQL连接对象
        _pDbCfg->Ip.c_str(),                 // 数据库IP地址
        _pDbCfg->User.c_str(),               // 数据库用户名
        _pDbCfg->Password.c_str(),           // 数据库密码
        _pDbCfg->DatabaseName.c_str(),       // 数据库名称
        _pDbCfg->Port,                       // 数据库端口
        nullptr,                             // 默认Unix套接字，Windows下为NULL
        CLIENT_FOUND_ROWS                    // 客户端标志，返回匹配行而不是影响行的数量
    );

    // 检查连接是否有错误
    int mysqlerrno = CheckMysqlError();
    if (mysqlerrno > 0)
    {
        // 如果有错误，断开连接并返回false
        Disconnect();
        return false;
    }

    // 不需要手动关闭自动提交模式，底层会在START TRANSACTION之后自动COMMIT
    // mysql_autocommit(mysql(), 0);

    // 发送一个ping请求到服务器以确保连接有效
    mysql_ping(_pMysql);
    mysqlerrno = CheckMysqlError();
    if (mysqlerrno > 0)
    {
        // 如果有错误，断开连接并返回false
        Disconnect();
        return false;
    }

    // 输出调试信息，记录成功连接的数据库IP和端口
    LOG_DEBUG("MysqlConnector::Connect: successfully! addr:" << _pDbCfg->Ip.c_str() << ":" << _pDbCfg->Port);

    // 初始化预处理语句
    InitStmts();

    // 标记连接已运行
    _isRunning = true;

    // 返回true表示成功连接
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
    // 创建一个 SQL 插入语句，并将其与数据库语句键 DatabaseStmtKey::StmtCreate 关联
    DatabaseStmt* stmt = CreateStmt("insert into player ( sn, account, name, savetime, createtime ) value ( ?, ?, ?, now(), now() )");
    _mapStmt.insert(std::make_pair(DatabaseStmtKey::StmtCreate, stmt));

    // 创建一个 SQL 更新语句，并将其与数据库语句键 DatabaseStmtKey::StmtSave 关联
    stmt = CreateStmt("update player set base=?, misc=?,savetime=now() where sn = ?");
    _mapStmt.insert(std::make_pair(DatabaseStmtKey::StmtSave, stmt));

    // 记录初始化成功的日志
    LOG_DEBUG("\tMysqlConnector::InitStmts successfully!");
}

void MysqlConnector::CleanStmts()
{
    // 关闭并清理所有存储在 _mapStmt 中的语句
    for (auto one : _mapStmt)
    {
        one.second->Close();
    }

    // 清空语句映射
    _mapStmt.clear();
}

DatabaseStmt* MysqlConnector::GetStmt(DatabaseStmtKey stmtKey)
{
    // 查找特定键的语句
    const auto iter = _mapStmt.find(stmtKey);
    if (iter == _mapStmt.end())
        return nullptr; // 如果找不到，返回 nullptr

    return iter->second; // 返回找到的语句
}

DatabaseStmt* MysqlConnector::CreateStmt(const char* sql) const
{
    int str_len = strlen(sql); // 计算 SQL 语句的长度
    DatabaseStmt* stmt = new DatabaseStmt(); // 创建新的数据库预处理语句对象
    int param_count = 0; // 初始化参数计数器

    stmt->stmt = mysql_stmt_init(_pMysql); // 初始化 MySQL 预处理语句
    if (mysql_stmt_prepare(stmt->stmt, sql, str_len) != 0)
    {
        // 如果准备预处理语句失败，记录错误日志并返回 nullptr
        LOG_ERROR("create_stmt error, " << LOG4CPLUS_C_STR_TO_TSTRING(mysql_stmt_error(stmt->stmt)));
        return nullptr;
    }

    // 统计 SQL 预处理语句中的参数数量
    for (int i = 0; i < str_len; i++)
    {
        if ((sql[i] == '?') || (sql[i] == '@'))
            param_count++;
    }

    if (param_count > 0)
    {
        // 如果有参数，初始化绑定数组和绑定缓冲区
        stmt->bind = new MYSQL_BIND[param_count];
        memset(stmt->bind, 0, sizeof(MYSQL_BIND) * param_count);
        stmt->bind_buffer = new char[MAX_BIND_BUFFER];
    }
    else
    {
        // 如果没有参数，绑定数组和缓冲区设为 nullptr
        stmt->bind = nullptr;
        stmt->bind_buffer = nullptr;
    }

    return stmt; // 返回创建的预处理语句对象
}

// 清除语句的绑定索引和绑定缓冲区索引
void MysqlConnector::ClearStmtParam(DatabaseStmt* stmt)
{
    // 清除语句的绑定索引和绑定缓冲区索引
    stmt->bind_index = 0;
    stmt->bind_buffer_index = 0;
}

bool MysqlConnector::ExecuteStmt(DatabaseStmt* stmt, my_ulonglong& affected_rows)
{
    MYSQL_STMT* st = stmt->stmt; // 获取 MySQL 语句指针
    if (stmt->bind != nullptr)
        mysql_stmt_bind_param(stmt->stmt, stmt->bind); // 绑定参数

    if (mysql_stmt_execute(st) != 0)
    {
        // 如果执行失败，记录错误日志并返回 false
        LOG_ERROR("execute_stmt error, " << LOG4CPLUS_C_STR_TO_TSTRING(mysql_stmt_error(st)));
        return false;
    }

    // 获取受影响的行数
    affected_rows = mysql_stmt_affected_rows(st);
    return true; // 执行成功，返回 true
}

bool MysqlConnector::ExecuteStmt(DatabaseStmt* stmt)
{
    MYSQL_STMT* st = stmt->stmt; // 获取 MySQL 语句指针
    if (stmt->bind != nullptr)
        mysql_stmt_bind_param(stmt->stmt, stmt->bind); // 绑定参数

    if (mysql_stmt_execute(st) != 0)
    {
        // 如果执行失败，记录错误日志并返回 false
        LOG_ERROR("execute_stmt error, " << LOG4CPLUS_C_STR_TO_TSTRING(mysql_stmt_error(st)));
        return false;
    }

    return true; // 执行成功，返回 true
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
    MYSQL_BIND* pBind = &stmt->bind[stmt->bind_index]; // 获取当前绑定位置的指针

    pBind->buffer_type = MYSQL_TYPE_LONGLONG; // 设置参数类型为无符号长整型
    pBind->buffer = &stmt->bind_buffer[stmt->bind_buffer_index]; // 绑定缓冲区指针
    pBind->is_unsigned = true; // 标记为无符号

    *static_cast<uint64*>(pBind->buffer) = val; // 将参数值复制到缓冲区

    stmt->bind_index++; // 更新绑定索引
    stmt->bind_buffer_index += sizeof(uint64); // 更新缓冲区索引
}

void MysqlConnector::AddParamInt(DatabaseStmt* stmt, int val)
{
    MYSQL_BIND* pBind = &stmt->bind[stmt->bind_index]; // 获取当前绑定位置的指针

    pBind->buffer_type = MYSQL_TYPE_LONG; // 设置参数类型为整型
    pBind->buffer = &stmt->bind_buffer[stmt->bind_buffer_index]; // 绑定缓冲区指针
    pBind->is_unsigned = false; // 标记为有符号

    *static_cast<long*>(pBind->buffer) = val; // 将参数值复制到缓冲区

    stmt->bind_index++; // 更新绑定索引
    stmt->bind_buffer_index += sizeof(long); // 更新缓冲区索引
}

void MysqlConnector::AddParamStr(DatabaseStmt* stmt, const char* val)
{
    MYSQL_BIND* pBind = &stmt->bind[stmt->bind_index]; // 获取当前绑定位置的指针
    int len = strlen(val); // 获取字符串长度
    if (len >= MAX_BIND_STR)
    {
        LOG_ERROR("add_str_param error, str size:" << len << " out of limit:" << MAX_BIND_STR); // 打印错误日志
        len = MAX_BIND_STR - 1; // 限制字符串长度
    }

    pBind->buffer_type = MYSQL_TYPE_STRING; // 设置参数类型为字符串
    pBind->buffer = &stmt->bind_buffer[stmt->bind_buffer_index]; // 绑定缓冲区指针
    pBind->length = (unsigned long*)& stmt->bind_buffer[stmt->bind_buffer_index + len + 1]; // 设置字符串长度指针

    engine_strncpy((char*)pBind->buffer, len + 1, val, len + 1); // 复制字符串到缓冲区
    *(pBind->length) = len; // 设置字符串长度
    pBind->buffer_length = len; // 设置缓冲区长度

    stmt->bind_index++; // 更新绑定索引
    stmt->bind_buffer_index += (len + 1 + sizeof(unsigned long*)); // 更新缓冲区索引
}
