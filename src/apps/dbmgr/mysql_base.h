#pragma once

#include "libserver/common.h"
#include "libserver/yaml.h"

#include <mysql/mysql.h>

class MysqlBase
{
public:
    bool ConnectInit();          // ��ʼ�������ӵ����ݿ�
    virtual void Disconnect();   // �Ͽ������ݿ������

    int CheckMysqlError() const; // ��鲢����MySQL�������

    bool Query(const char* sql, my_ulonglong& affected_rows); // ִ��SQL��ѯ��������Ӱ�������
    MYSQL_ROW Fetch() const;     // ��ȡ��ѯ�������һ��

    static int GetInt(MYSQL_ROW row, int index);                  // ��ȡ�����ֶ�ֵ
    static unsigned int GetUint(MYSQL_ROW row, int index);        // ��ȡ�޷��������ֶ�ֵ
    static uint64 GetUint64(MYSQL_ROW row, int index);            // ��ȡ64λ�޷��������ֶ�ֵ
    static char* GetString(MYSQL_ROW row, int index);             // ��ȡ�ַ����ֶ�ֵ
    int GetBlob(MYSQL_ROW row, int index, char* buf, unsigned long size) const; // ��ȡ�����������ֶ�ֵ
    void GetBlob(MYSQL_ROW row, int index, std::string& protoStr) const;        // ��ȡ�����������ֶ�ֵ���洢���ַ�����

protected:
    DBConfig* _pDbCfg{ nullptr };   // ���ݿ�����ָ��

    // MySQL���ָ��
    MYSQL* _pMysql{ nullptr };        // MySQL����ָ��
    MYSQL_RES* _pMysqlRes{ nullptr }; // MySQL�����ָ��
    int _numFields{ 0 };              // �ֶ�����
    MYSQL_FIELD* _pMysqlFields{ nullptr }; // �ֶ���Ϣָ��
};

