#pragma once

#include "disposable.h"
#include "system.h"
#include "common.h"

#include <list>
#include <random>

// SystemManager管理着 MessageSystem 和 EntitySystem
// 协调和管理各个系统组件的生命周期和操作
class EntitySystem;
class MessageSystem;

class SystemManager : virtual public IDisposable
{
public:
    SystemManager();                                        // 构造函数
    void InitComponent();                                   // 初始化组件
    virtual void Update();                                  // 更新系统
    void Dispose() override;                                // 释放资源
    MessageSystem* GetMessageSystem() const;                // 获取MessageSystem
    EntitySystem* GetEntitySystem() const;                  // 获取EntitySystem
    std::default_random_engine* GetRandomEngine() const;    // 获取随机数引擎

protected:
    MessageSystem* _pMessageSystem;                 // 消息系统指针
    EntitySystem* _pEntitySystem;                   // 实体系统指针
    std::list<ISystem*> _systems;                   // 系统列表
    std::default_random_engine* _pRandomEngine;     // 随机数引擎指针
};
