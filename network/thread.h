/* 
说明： 
功能：网络服务线程类
作者：QHXRPG
日期：2024/5/24
*/

#pragma once

#include <thread>
#include "disposable.h"

class Thread : public IDisposable
{
public:
    virtual bool Start();   // 让子类去重写自己的启动函数
    void Stop();
    void Dispose() override;
    virtual void Update();  // 让子类去重写自己的帧函数

    bool IsRun() const
    {
        return _isRun;
    }

protected:
    bool _isRun { false};
    std::thread _thread;
};