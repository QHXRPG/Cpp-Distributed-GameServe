#pragma once

#include "libserver/common.h"
#include "libserver/yaml.h"

#include <mysql/mysql.h>

// MysqlBase���װ����MySQL���ݿ⽻���Ļ�������
class MysqlBase
{
public:
    
    bool ConnectInit();                                                             // ��ʼ��MySQL����         
    virtual void Disconnect();                                                      // �Ͽ�MySQL����
    int CheckMysqlError() const;                                                    // ���MySQL�Ƿ���ڴ���
    bool Query(const char* sql, my_ulonglong& affected_rows);                       // ִ��SQL��ѯ
    
    MYSQL_ROW Fetch() const;                                                        // ��ȡ��ѯ�������һ��

    
    static int GetInt(MYSQL_ROW row, int index);                                    // �Ӳ�ѯ�����һ���л�ȡ�����ֶ�ֵ
    static unsigned int GetUint(MYSQL_ROW row, int index);                          // �Ӳ�ѯ�����һ���л�ȡ�޷��������ֶ�ֵ
    static uint64 GetUint64(MYSQL_ROW row, int index);                              // �Ӳ�ѯ�����һ���л�ȡ�޷���64λ�����ֶ�ֵ
    static char* GetString(MYSQL_ROW row, int index);                               // �Ӳ�ѯ�����һ���л�ȡ�ַ����ֶ�ֵ
    int GetBlob(MYSQL_ROW row, int index, char* buf, unsigned long size) const;     // �Ӳ�ѯ�����һ���л�ȡBlob�ֶ�ֵ���洢����������
    void GetBlob(MYSQL_ROW row, int index, std::string& protoStr) const;            // �Ӳ�ѯ�����һ���л�ȡBlob�ֶ�ֵ���洢��std::string��

protected:
    DBConfig* _pDbCfg{ nullptr };           // ���ݿ�����

    // MySQL������ر���
    MYSQL* _pMysql{ nullptr };              // MySQL���Ӿ��
    MYSQL_RES* _pMysqlRes{ nullptr };       // MySQL��ѯ�����
    int _numFields{ 0 };                    // ��ѯ����е��ֶ���
    MYSQL_FIELD* _pMysqlFields{ nullptr };  // ��ѯ����е��ֶ���Ϣ
};
