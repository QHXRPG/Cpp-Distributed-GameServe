#pragma once

#include <mutex>

#include "common.h"
#include "thread.h"
#include "singleton.h"
#include <map>

class ThreadMgr : public Singleton<ThreadMgr>
{
private:
    uint64 _lastThraedSn {0};  // ʵ���̶߳�����֣�ʵ���̸߳��ؾ���
    std::mutex _thread_lock;
    std::map<uint64, Thread*> _threads; //�̳߳�

public:
    ThreadMgr();
    void StartAllThread();
    bool IsGameLoop();
    void NewThread();
    void AddObjToThread(ThreadObject* obj);
    void Dispose();
};