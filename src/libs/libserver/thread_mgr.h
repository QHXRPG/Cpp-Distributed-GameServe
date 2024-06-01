#pragma once

#include <mutex>

#include "common.h"
#include "thread.h"
#include "singleton.h"
#include <map>

class ThreadMgr : public Singleton<ThreadMgr>
{
private:
    uint64 _lastThraedSn {0};  // 实现线程对象均分，实现线程负载均衡
    std::mutex _thread_lock;
    std::map<uint64, Thread*> _threads; //线程池

public:
    ThreadMgr();
    void StartAllThread();
    bool IsGameLoop();
    void NewThread();
    void AddObjToThread(ThreadObject* obj);
    void Dispose();
};