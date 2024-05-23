/* 
说明： 
功能：网络监听
作者：QHXRPG
日期：2024/5/23
*/
#pragma once

#include "network.h"

class NetworkListen : public Network
{
protected:
    virtual int Accept();

public:
    bool Listen(std::string ip, int port);
    bool Update();
};