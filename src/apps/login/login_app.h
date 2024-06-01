#pragma once
#include "libserver/server_app.h"

class LoginApp : public ServerApp 
{
public:
    explicit LoginApp() 
    : ServerApp(APP_TYPE::APP_LOGIN) // 把服务类型传给 服务基类ServerApp
    { }

    void InitApp() override;
};

