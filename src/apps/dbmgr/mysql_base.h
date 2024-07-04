#pragma once

#include "libserver/common.h"
#include "libserver/yaml.h"

#include <mysql/mysql.h>

class MysqlBase
{
public:
    bool ConnectInit();          // 初始化并连接到数据库
    virtual void Disconnect();   // 断开与数据库的连接

    int CheckMysqlError() const; // 检查并返回MySQL错误代码

    bool Query(const char* sql, my_ulonglong& affected_rows); // 执行SQL查询并返回受影响的行数
    MYSQL_ROW Fetch() const;     // 获取查询结果的下一行

    static int GetInt(MYSQL_ROW row, int index);                  // 获取整型字段值
    static unsigned int GetUint(MYSQL_ROW row, int index);        // 获取无符号整型字段值
    static uint64 GetUint64(MYSQL_ROW row, int index);            // 获取64位无符号整型字段值
    static char* GetString(MYSQL_ROW row, int index);             // 获取字符串字段值
    int GetBlob(MYSQL_ROW row, int index, char* buf, unsigned long size) const; // 获取二进制数据字段值
    void GetBlob(MYSQL_ROW row, int index, std::string& protoStr) const;        // 获取二进制数据字段值并存储在字符串中

protected:
    DBConfig* _pDbCfg{ nullptr };   // 数据库配置指针

    // MySQL相关指针
    MYSQL* _pMysql{ nullptr };        // MySQL连接指针
    MYSQL_RES* _pMysqlRes{ nullptr }; // MySQL结果集指针
    int _numFields{ 0 };              // 字段数量
    MYSQL_FIELD* _pMysqlFields{ nullptr }; // 字段信息指针
};

