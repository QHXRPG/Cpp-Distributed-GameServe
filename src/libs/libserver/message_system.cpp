#include "message_system.h"
#include "system_manager.h"
#include "packet.h"
#include "message_component.h"
#include "entity_system.h"
#include "component.h"

// ���캯������ʼ��ϵͳ������ָ��
MessageSystem::MessageSystem(SystemManager* pMgr)
{
    _systemMgr = pMgr;
}

// ��Դ�ͷź�����Ŀǰδʵ�־����߼�
void MessageSystem::Dispose()
{
    // TODO: �ͷ���Դ�ľ���ʵ��
}

// ������ӵ��������б���
void MessageSystem::AddPacketToList(Packet* pPacket)
{
    std::lock_guard<std::mutex> guard(_packet_lock); // �����Ա�����������
    _cachePackets.GetWriterCache()->emplace_back(pPacket); // ������ӵ�д������
}

// ���º������������д�����İ�
void MessageSystem::Update(EntitySystem* pEntities)
{
    _packet_lock.lock(); // �����Ա�����������
    if (_cachePackets.CanSwap()) // ����Ƿ���Խ�������
    {
        _cachePackets.Swap(); // ������д����
    }
    _packet_lock.unlock(); // ����

    // ���������Ϊ�գ�ֱ�ӷ���
    if (_cachePackets.GetReaderCache()->size() == 0)
        return;

    // ��ȡ��Ϣ����ļ���
    auto pCollections = pEntities->GetComponentCollections<MessageComponent>();
    if (pCollections == nullptr) 
    {
        _cachePackets.GetReaderCache()->clear(); // ���û����Ϣ�������ն�����
        return;
    }

    pCollections->Swap(); // ������Ϣ����Ļ���

    // ��ȡ������Ϣ���
    auto lists = pCollections->GetAll();
    auto packetLists = _cachePackets.GetReaderCache(); // ��ȡ�������еİ��б�
    for (auto iter = packetLists->begin(); iter != packetLists->end(); ++iter)
    {
        Process((*iter), lists); // ����ÿ����
    }

    _cachePackets.GetReaderCache()->clear(); // ��ն�����
}

// ������ľ�̬����
void MessageSystem::Process(Packet* pPacket, std::map<uint64, IComponent*>& lists)
{
    // ����������Ϣ���
    for (auto iter = lists.begin(); iter != lists.end(); ++iter)
    {
        MessageComponent* pMsgComponent = static_cast<MessageComponent*>(iter->second); // �����ת��Ϊ��Ϣ���
        if (pMsgComponent->IsFollowMsgId(pPacket)) // �������Ƿ��ע�ð�����ϢID
        {
            pMsgComponent->ProcessPacket(pPacket); // �����
        }
    }
}
