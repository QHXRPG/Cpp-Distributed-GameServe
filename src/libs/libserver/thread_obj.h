#pragma once

#include "disposable.h"

// 每个线程包裹类都是一个Actor
class ThreadObject : public IDisposable
{
public:
    virtual ~ThreadObject () {}

    // 初始化函数
    virtual bool Init() = 0;

    // 注册感兴趣的协议
    virtual void RegisterMsgFunction() = 0;

    // 帧函数
    virtual void Update() = 0;

    bool IsActive() const;

protected:
    bool _active {true};
};