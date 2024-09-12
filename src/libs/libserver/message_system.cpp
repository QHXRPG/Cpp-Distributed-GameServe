#include "message_system.h"
#include "system_manager.h"
#include "packet.h"
#include "message_component.h"
#include "entity_system.h"
#include "component.h"

// 构造函数，初始化系统管理器指针
MessageSystem::MessageSystem(SystemManager* pMgr)
{
    _systemMgr = pMgr;
}

// 资源释放函数，目前未实现具体逻辑
void MessageSystem::Dispose()
{
    // TODO: 释放资源的具体实现
}

// 将包添加到待处理列表中
void MessageSystem::AddPacketToList(Packet* pPacket)
{
    std::lock_guard<std::mutex> guard(_packet_lock); // 加锁以保护共享数据
    _cachePackets.GetWriterCache()->emplace_back(pPacket); // 将包添加到写缓存中
}

// 更新函数，处理所有待处理的包
void MessageSystem::Update(EntitySystem* pEntities)
{
    _packet_lock.lock(); // 加锁以保护共享数据
    if (_cachePackets.CanSwap()) // 检查是否可以交换缓存
    {
        _cachePackets.Swap(); // 交换读写缓存
    }
    _packet_lock.unlock(); // 解锁

    // 如果读缓存为空，直接返回
    if (_cachePackets.GetReaderCache()->size() == 0)
        return;

    // 获取消息组件的集合
    auto pCollections = pEntities->GetComponentCollections<MessageComponent>();
    if (pCollections == nullptr) 
    {
        _cachePackets.GetReaderCache()->clear(); // 如果没有消息组件，清空读缓存
        return;
    }

    pCollections->Swap(); // 交换消息组件的缓存

    // 获取所有消息组件
    auto lists = pCollections->GetAll();
    auto packetLists = _cachePackets.GetReaderCache(); // 获取读缓存中的包列表
    for (auto iter = packetLists->begin(); iter != packetLists->end(); ++iter)
    {
        Process((*iter), lists); // 处理每个包
    }

    _cachePackets.GetReaderCache()->clear(); // 清空读缓存
}

// 处理包的静态函数
void MessageSystem::Process(Packet* pPacket, std::map<uint64, IComponent*>& lists)
{
    // 遍历所有消息组件
    for (auto iter = lists.begin(); iter != lists.end(); ++iter)
    {
        MessageComponent* pMsgComponent = static_cast<MessageComponent*>(iter->second); // 将组件转换为消息组件
        if (pMsgComponent->IsFollowMsgId(pPacket)) // 检查组件是否关注该包的消息ID
        {
            pMsgComponent->ProcessPacket(pPacket); // 处理包
        }
    }
}
