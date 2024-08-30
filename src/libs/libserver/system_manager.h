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
    SystemManager();
    void InitComponent();

    virtual void Update();
    void Dispose() override;

    MessageSystem* GetMessageSystem() const;
    EntitySystem* GetEntitySystem() const;

    std::default_random_engine* GetRandomEngine() const;

protected:
    MessageSystem* _pMessageSystem;
    EntitySystem* _pEntitySystem;

    std::list<ISystem*> _systems;

    std::default_random_engine* _pRandomEngine;
};
