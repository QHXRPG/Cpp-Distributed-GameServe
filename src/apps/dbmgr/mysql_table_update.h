#pragma once

#include "mysql_base.h"

#include <functional>

class MysqlTableUpdate :public MysqlBase, public Singleton<MysqlTableUpdate>
{
public:
    MysqlTableUpdate();
    virtual ~MysqlTableUpdate() = default;

    void Check();

private:
    bool CreateDatabaseIfNotExist();

    // ���DB���ݣ����µ����°汾
    bool UpdateToVersion();
    bool Update00();

private:
    // update
    typedef std::function<bool(void)> OnUpdate;
    std::vector<OnUpdate> _update_func;

    int const _version = 0;
};
