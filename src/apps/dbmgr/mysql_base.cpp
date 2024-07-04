#include "mysql_base.h"

#include "libserver/log4_help.h"

bool MysqlBase::ConnectInit()
{
    Disconnect();

    // 由mysql库自行new出Mysql对象的方式
    _pMysql = mysql_init(nullptr);
    if (_pMysql == nullptr)
    {
        CheckMysqlError();
        LOG_ERROR("mysql_init == nullptr");
        return false;
    }

    // 设置连接等待时间，这里设置的是10秒
    int outtime = 10;
    mysql_options(_pMysql, MYSQL_OPT_CONNECT_TIMEOUT, &outtime);

    // 设置如果连接失败 不自动连接
    bool reConnect = false;
    mysql_options(_pMysql, MYSQL_OPT_RECONNECT, &reConnect);

    _numFields = 0;
    _pMysqlFields = nullptr;
    _pMysqlRes = nullptr;

    return true;
}

void MysqlBase::Disconnect()
{
    // 将指向结果集的指针置为空
    _pMysqlFields = nullptr;

    // 如果结果集不为空,则释放结果集
    if (_pMysqlRes != nullptr)
    {
        mysql_free_result(_pMysqlRes);
        _pMysqlRes = nullptr;
    }

    // 如果MySQL连接对象不为空,则关闭连接
    if (_pMysql != nullptr)
    {
        mysql_close(_pMysql);
        _pMysql = nullptr;
    }
}

int MysqlBase::CheckMysqlError() const
{
    const int mysqlerrno = mysql_errno(_pMysql);
    if (mysqlerrno > 0)
    {
        LOG_ERROR("MysqlConnector::CheckError. mysql_errno=" << mysql_errno(_pMysql) << ", mysql_error=" << mysql_error(_pMysql));
        return mysqlerrno;
    }

    return 0;
}

bool MysqlBase::Query(const char* sql, my_ulonglong& affected_rows)
{
    // 如果之前有结果集,则先释放掉
    if (nullptr != _pMysqlRes)
    {
        mysql_free_result(_pMysqlRes);
        _pMysqlRes = nullptr;
    }

    // 执行SQL查询
    if (mysql_query(_pMysql, sql) != 0)
    {
        // 如果查询失败,记录错误日志并返回false
        LOG_ERROR("Query error:" << mysql_error(_pMysql) << " sql:" << sql);
        return false;
    }

    // 获取查询结果集
    _pMysqlRes = mysql_store_result(_pMysql);
    if (_pMysqlRes != nullptr)
    {
        // 如果有结果集,记录字段数和字段指针
        _numFields = mysql_num_fields(_pMysqlRes);
        _pMysqlFields = mysql_fetch_fields(_pMysqlRes);
    }

    // 获取行数
    affected_rows = mysql_affected_rows(_pMysql);

    // 返回true表示查询成功
    return true;
}


MYSQL_ROW MysqlBase::Fetch() const
{
    if (_pMysqlRes == nullptr)
        return nullptr;

    return mysql_fetch_row(_pMysqlRes);
}


int MysqlBase::GetInt(MYSQL_ROW row, int index)
{
    if (row[index] == nullptr)
    {
        LOG_ERROR("!!! Failed. MysqlConnector::GetInt");
        return 0;
    }

    return atoi(row[index]);
}

uint64 MysqlBase::GetUint64(MYSQL_ROW row, int index)
{
    if (row[index] == nullptr)
    {
        LOG_ERROR("!!! Failed. MysqlConnector::GetUint64");
        return 0;
    }

    return  atoll(row[index]);
}

unsigned int MysqlBase::GetUint(MYSQL_ROW row, int index)
{
    if (row[index] == nullptr)
    {
        LOG_ERROR("!!! Failed. MysqlConnector::GetUInt64");
        return 0;
    }

    return strtoul(row[index], nullptr, 10);
}

char* MysqlBase::GetString(MYSQL_ROW row, int index)
{
    if (row[index] == nullptr)
    {
        LOG_ERROR("!!! Failed. MysqlConnector::GetString");
        return nullptr;
    }

    return row[index];
}

int MysqlBase::GetBlob(MYSQL_ROW row, int index, char* buf, unsigned long size) const
{
    unsigned int l = size > 0 ? size : 0;
    if (row[index] == nullptr)
    {
        LOG_ERROR("!!! Failed. MysqlConnector::GetBlob");
        return 0;
    }

    unsigned long* lengths = mysql_fetch_lengths(_pMysqlRes);
    if (lengths[index] < l)
        l = lengths[index];

    memcpy(buf, row[index], l);
    return l;
}

void MysqlBase::GetBlob(MYSQL_ROW row, int index, std::string& protoStr) const
{
    unsigned long* pLengths = mysql_fetch_lengths(_pMysqlRes);
    long blobLength = pLengths[index];

    if (blobLength <= 0)
    {
        protoStr = "";
        return;
    }

    char* blobByte = new char[blobLength + 1];
    GetBlob(row, index, blobByte, blobLength);
    blobByte[blobLength] = '\0';

    protoStr = blobByte;
    delete[] blobByte;
}
