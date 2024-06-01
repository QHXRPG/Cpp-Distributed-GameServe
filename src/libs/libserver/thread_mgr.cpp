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

// ����Ƿ����߳��ڹ���������߳�ȫ���˳���������˳�
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

// ������߳�
void ThreadMgr::NewThread()
{
    std::lock_guard<std::mutex> guard(_thread_lock);
    Thread* pThread = new Thread();
    _threads.insert(std::make_pair(pThread->GetSN(), pThread)); //�����߳��������һ���߳�id
}

// ��ThreadObject ƽ���ؼ��뵽�����߳��У�ʵ�ָ��ؾ���
void ThreadMgr::AddObjToThread(ThreadObject *obj)
{
    std::lock_guard<std::mutex> guard(_thread_lock);

    // ����֮ǰ��ʼ��
    if(! obj->Init())
    {
        std::cout << "ThreadMgr::AddObjToThread() faild: ThreadObject��ʼ��ʧ��"<<std::endl;
        return;
    }

    // �ҵ���һ�α�����obj���߳�
    auto iter = _threads.begin();
    if(_lastThraedSn >0)
    {
        iter = _threads.find(_lastThraedSn);
    }

    // ����̳߳��л�û���߳�
    if(iter == _threads.end())
    {
        std::cout<<"ThreadMgr::AddObjToThread() faild: �̳߳���û���߳�"<<std::endl;
        return;
    }

    // ȡ����һ�α�����obj���߳� ����һ����߳�
    do
    {
        iter ++;

        // ���ֻ��һ���߳�
        if(iter == _threads.end())
            iter = _threads.begin();
    } while (! iter->second->IsRun());

    Thread* pThread = iter->second;
    pThread->AddThreadObj(obj);
    _lastThraedSn = pThread->GetSN();  // ���� _lastThraedSn
    
}

// ���Լ������µ������̷߳��ʹ�connection����������Packet����
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
