/* 
说明： 
功能：客户端线程类
作者：QHXRPG
日期：2024/5/24
*/
#pragma once
#include "network/thread.h"

class Client;

class ClientThread : public Thread
{
public:
    explicit ClientThread(int msgCount);
    void Update() override;
    void Dispose() override;

private:
    Client * _Client{nullptr};
    int _msgCount;
};