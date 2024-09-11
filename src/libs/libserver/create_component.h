// 预防头文件被重复包含
#pragma once

// 包含必要的头文件
#include "entity.h"
#include "system.h"
#include "message_system.h"

// 前向声明
class Packet;

// CreateComponentC 类定义，继承自 Entity 和 IAwakeFromPoolSystem
class CreateComponentC : public Entity<CreateComponentC>, public IAwakeFromPoolSystem<>
{
public:
    // 从对象池中唤醒的方法
    void AwakeFromPool() override;
    // 返回对象池的方法
    void BackToPool() override;

private:
    // 处理创建组件的函数
    void HandleCreateComponent(Packet* pPacket) const;
    // 处理移除组件的函数
    void HandleRemoveComponent(Packet* pPacket);
};