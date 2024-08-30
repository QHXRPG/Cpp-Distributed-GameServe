#pragma once

#include "libserver/common.h"
#include "libserver/yaml.h"

#include <mysql/mysql.h>

// MysqlBase类封装了与MySQL数据库交互的基本操作
class MysqlBase
{
public:
    
    bool ConnectInit();                                                             // 初始化MySQL连接         
    virtual void Disconnect();                                                      // 断开MySQL连接
    int CheckMysqlError() const;                                                    // 检查MySQL是否存在错误
    bool Query(const char* sql, my_ulonglong& affected_rows);                       // 执行SQL查询
    
    MYSQL_ROW Fetch() const;                                                        // 获取查询结果的下一行

    
    static int GetInt(MYSQL_ROW row, int index);                                    // 从查询结果的一行中获取整型字段值
    static unsigned int GetUint(MYSQL_ROW row, int index);                          // 从查询结果的一行中获取无符号整型字段值
    static uint64 GetUint64(MYSQL_ROW row, int index);                              // 从查询结果的一行中获取无符号64位整型字段值
    static char* GetString(MYSQL_ROW row, int index);                               // 从查询结果的一行中获取字符串字段值
    int GetBlob(MYSQL_ROW row, int index, char* buf, unsigned long size) const;     // 从查询结果的一行中获取Blob字段值并存储到缓冲区中
    void GetBlob(MYSQL_ROW row, int index, std::string& protoStr) const;            // 从查询结果的一行中获取Blob字段值并存储到std::string中

protected:
    DBConfig* _pDbCfg{ nullptr };           // 数据库配置

    // MySQL连接相关变量
    MYSQL* _pMysql{ nullptr };              // MySQL连接句柄
    MYSQL_RES* _pMysqlRes{ nullptr };       // MySQL查询结果集
    int _numFields{ 0 };                    // 查询结果中的字段数
    MYSQL_FIELD* _pMysqlFields{ nullptr };  // 查询结果中的字段信息
};
