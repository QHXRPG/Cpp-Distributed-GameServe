#pragma once

#include "disposable.h"
#include "system.h"
#include "common.h"

#include <list>
#include <random>

// SystemManager������ MessageSystem �� EntitySystem
// Э���͹������ϵͳ������������ںͲ���

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
