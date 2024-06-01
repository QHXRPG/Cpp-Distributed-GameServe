#include "thread_mgr.h"
#include "common.h"
#include <iostream>

ThreadMgr::ThreadMgr()
{
}

void ThreadMgr::StartAllThread()
{
    auto iter = _threads.begin();
    while(iter != _threads.end())
    {
        iter->second->Start();
        iter ++;
    }
}

// 检查是否有线程在工作，如果线程全部退出，则进程退出
bool ThreadMgr::IsGameLoop()
{
    auto iter = _threads.begin();
    while(iter != _threads.end())
    {
        if(iter->second->IsRun())
            return true;
    }
    return false;
}

// 添加新线程
void ThreadMgr::NewThread()
{
    std::lock_guard<std::mutex> guard(_thread_lock);
    Thread* pThread = new Thread();
    _threads.insert(std::make_pair(pThread->GetSN(), pThread)); //给新线程随机分配一个线程id
}

// 将ThreadObject 平均地加入到各个线程中，实现负载均衡
void ThreadMgr::AddObjToThread(ThreadObject *obj)
{
    std::lock_guard<std::mutex> guard(_thread_lock);

    // 加入之前初始化
    if(! obj->Init())
    {
        std::cout << "ThreadMgr::AddObjToThread() faild: ThreadObject初始化失败"<<std::endl;
        return;
    }

    // 找到上一次被加入obj的线程
    auto iter = _threads.begin();
    if(_lastThraedSn >0)
    {
        iter = _threads.find(_lastThraedSn);
    }

    // 如果线程池中还没有线程
    if(iter == _threads.end())
    {
        std::cout<<"ThreadMgr::AddObjToThread() faild: 线程池中没有线程"<<std::endl;
        return;
    }

    // 取到上一次被加入obj的线程 的下一个活动线程
    do
    {
        iter ++;

        // 如果只有一个线程
        if(iter == _threads.end())
            iter = _threads.begin();
    } while (! iter->second->IsRun());

    Thread* pThread = iter->second;
    pThread->AddThreadObj(obj);
    _lastThraedSn = pThread->GetSN();  // 更新 _lastThraedSn
    
}

// 向自己管理下的所有线程发送从connection对象发送来的Packet数据
void ThreadMgr::AddPacket(Packet *pPacket)
{
    std::lock_guard<std::mutex> guard(_thread_lock);
    for(auto iter = _threads.begin(); iter!=_threads.end(); ++iter)
    {
        Thread* pThread = iter->second;
        pThread->AddPacket(pPacket);
    }
}

void ThreadMgr::Dispose()
{
    auto iter = _threads.begin();
    while(iter != _threads.end())
    {
        Thread* pThread = iter->second;
        pThread->Dispose();
        delete pThread;
        iter ++;
    }
}
