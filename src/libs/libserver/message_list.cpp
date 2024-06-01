#include "message_list.h"
#include "iostream"
#include "packet.h"

// ��Ϣע��
void MessageList::RegisterFunction(int msgId, HandleFuntion function)
{
    std::lock_guard<std::mutex> guard(_msgMutex);
    _callbackHandle[msgId] = function;
}


// �жϸ�Э����Ƿ����Լ����ĵ�
bool MessageList::IsRegisteredMsg(int msgId)
{
    std::lock_guard<std::mutex> guard(_msgMutex);

    // ���Լ�����Ȥ����Ϣ������ture������false
    auto iter = _callbackHandle.find(msgId);
    if(iter == _callbackHandle.end())
        return false;
    else
        return true;
}


// ��Ϣ����
void MessageList::ProcessPacket()
{
    // ����һ����ʱ�����洢��Ϣ���У��������ٽ���
    std::list<Packet*> _tmpList;
    _msgMutex.lock();
    std::copy(_msgList.begin(), _msgList.end(), std::back_inserter(_tmpList));
    _msgList.clear();
    _msgMutex.unlock();

    for(auto packet : _tmpList)
    {
        const auto handleIter = _callbackHandle.find(packet->GetMsgId());
        if (handleIter == _callbackHandle.end())
        {
            std::cout << "����Ϣû�ж�Ӧ�ķ���ʵ��. msg id:" << packet->GetMsgId() << std::endl;
        }
        else
        {
            handleIter->second(packet);
        }
    }
    _tmpList.clear();
}


// ��Thread ���� Actor �İ��ӽ� _msgList, 
// ����İ��Ѿ������˴������Ǹ�Actor����Ȥ����Ϣ
void MessageList::AddPacket(Packet *pPacket)
{
    std::lock_guard<std::mutex> guard(_msgMutex);
    _msgList.push_back(pPacket);
}
