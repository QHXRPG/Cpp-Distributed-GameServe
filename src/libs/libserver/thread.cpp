#include "thread.h"
#include "global.h"

#include <iterator>

void ThreadObjectList::AddObject(ThreadObject* obj)
{
    std::lock_guard<std::mutex> guard(_obj_lock);

    // �ڼ���֮ǰ��ʼ��һ��
    if (!obj->Init())
    {
        std::cout << "AddObject Failed. ThreadObject init failed." << std::endl;
    }
    else
    {
        obj->RegisterMsgFunction();
        _objlist.GetAddCache()->emplace_back(obj);

        const auto pThread = dynamic_cast<Thread*>(this);
        if (pThread != nullptr)
            obj->SetThread(pThread);
    }
}

void ThreadObjectList::Update()
{
    _obj_lock.lock();
    
    // �ͷ���Ҫɾ���Ķ���
    if (_objlist.CanSwap())
    {
        auto pDelList = _objlist.Swap();
        for (auto pOne : pDelList)
        {
            pOne->Dispose();
            delete pOne;
        }
    }
    _obj_lock.unlock();

    _packet_lock.lock();
    if (_cachePackets.CanSwap())
    {
        _cachePackets.Swap();
    }
    _packet_lock.unlock();

    auto pList = _objlist.GetReaderCache();   // �õ������󻺳�����ָ��, 
    auto pMsgList = _cachePackets.GetReaderCache();   // �õ������ݻ�������ָ��

    for (auto iter = pList->begin(); iter != pList->end(); ++iter)
    {
        auto pObj = (*iter);
        for (auto itMsg = pMsgList->begin(); itMsg != pMsgList->end(); ++itMsg)
        {
            auto pPacket = (*itMsg);
            if (pObj->IsFollowMsgId(pPacket))
                pObj->ProcessPacket(pPacket);
        }

        pObj->Update();

        // �Ǽ���״̬��ɾ��
        if (!pObj->IsActive())
        {
            _objlist.GetRemoveCache()->emplace_back(pObj);
        }
    }

    pMsgList->clear();
    
}

void ThreadObjectList::AddPacketToList(Packet* pPacket)
{
    std::lock_guard<std::mutex> guard(_packet_lock);
    _cachePackets.GetWriterCache()->emplace_back(pPacket);
}

void ThreadObjectList::Dispose()
{
    _objlist.Dispose();
    _cachePackets.Dispose();
}

Thread::Thread()
{
    this->_isRun = true;
}

void Thread::Stop()
{
    if (!_isRun)
    {
        _isRun = false;
        if (_thread.joinable()) _thread.join();
    }
}

void Thread::Start()
{
    _isRun = true;
    _thread = std::thread([this]()
        {
            while (_isRun)
            {
                Update();
                std::this_thread::sleep_for(std::chrono::milliseconds(1));
            }
        });
}

bool Thread::IsRun() const
{
    return _isRun;
}

