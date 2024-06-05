#include "message_list.h"
#include <iterator>

#include "packet.h"
#include "thread_mgr.h"

void MessageCallBackFunctionInfo::AddPacket(Packet* pPacket)
{
    std::lock_guard<std::mutex> guard(_msgMutex);
    _msgList.push_back(pPacket);
}

void MessageCallBackFunction::RegisterFunction(int msgId, HandleFunction function)
{
    std::lock_guard<std::mutex> guard(_msgMutex);
    _callbackHandle[msgId] = function;
}

bool MessageCallBackFunction::IsFollowMsgId(Packet* packet)
{
    std::lock_guard<std::mutex> guard(_msgMutex);
    return _callbackHandle.find(packet->GetMsgId()) != _callbackHandle.end();
}

void MessageCallBackFunction::ProcessPacket()
{
    std::list<Packet*> tmpList;
    _msgMutex.lock();
    std::copy(_msgList.begin(), _msgList.end(), std::back_inserter(tmpList));
    _msgList.clear();
    _msgMutex.unlock();

    for (auto packet : tmpList)
    {
        const auto handleIter = _callbackHandle.find(packet->GetMsgId());
        if (handleIter == _callbackHandle.end())
        {
            std::cout << "packet is not hander. msg id;" << packet->GetMsgId() << std::endl;
        }
        else
        {
            handleIter->second(packet);
        }
    }

    tmpList.clear();
}

void MessageList::Dispose()
{
    if (_pCallBackFuns != nullptr)
    {
        delete _pCallBackFuns;
        _pCallBackFuns = nullptr;
    }
}

void MessageList::AttachCallBackHandler(MessageCallBackFunctionInfo* pCallback)
{
    _pCallBackFuns = pCallback;
}

bool MessageList::IsFollowMsgId(Packet* packet) const
{
    if (_pCallBackFuns == nullptr)
        return false;

    return _pCallBackFuns->IsFollowMsgId(packet);
}

void MessageList::ProcessPacket() const
{
    if (_pCallBackFuns == nullptr)
        return;

    _pCallBackFuns->ProcessPacket();
}

void MessageList::AddPacket(Packet* pPacket) const
{
    if (_pCallBackFuns == nullptr)
    {
        std::cout << "add packet failed. _pCallBackFuns == nullptr";
        return;
    }

    _pCallBackFuns->AddPacket(pPacket);
}

void MessageList::DispatchPacket(Packet* pPacket)
{
    ThreadMgr::GetInstance()->DispatchPacket(pPacket);
}

void MessageList::SendPacket(Packet* pPacket)
{
    ThreadMgr::GetInstance()->SendPacket(pPacket);
}
