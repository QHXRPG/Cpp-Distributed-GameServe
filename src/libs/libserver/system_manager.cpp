#include "system_manager.h"

#include "create_component.h"
#include "message_system.h"
#include "entity_system.h"
#include "update_system.h"
#include "console_thread_component.h"

#include <thread>

SystemManager::SystemManager()
{
    _pEntitySystem = new EntitySystem(this);    // 初始化实体系统
    _pMessageSystem = new MessageSystem(this);  // 初始化消息系统
    _systems.emplace_back(new UpdateSystem());  // 添加更新系统

    // 根据线程ID生成随机种子
    std::stringstream strStream;
    strStream << std::this_thread::get_id();
    std::string idstr = strStream.str();
    std::seed_seq seed1(idstr.begin(), idstr.end());
    std::minstd_rand0 generator(seed1);
    _pRandomEngine = new std::default_random_engine(generator());
}

void SystemManager::InitComponent()
{
    _pEntitySystem->AddComponent<CreateComponentC>();       // 添加创建组件
    _pEntitySystem->AddComponent<ConsoleThreadComponent>(); // 添加控制台线程组件
}

void SystemManager::Update()
{
    _pEntitySystem->Update();                   // 更新实体系统
    _pMessageSystem->Update(_pEntitySystem);    // 更新消息系统

    // 更新所有系统
    for (auto iter = _systems.begin(); iter != _systems.end(); ++iter)
    {
        (*iter)->Update(_pEntitySystem);
    }
}

void SystemManager::Dispose()
{
    // 释放所有系统
    for (auto one : _systems)
    {
        one->Dispose();
        delete one;
    }
    _systems.clear();

    // 释放随机数引擎
    delete _pRandomEngine;
    _pRandomEngine = nullptr;

    // 释放消息系统
    _pMessageSystem->Dispose();
    delete _pMessageSystem;
    _pMessageSystem = nullptr;

    // 释放实体系统
    _pEntitySystem->Dispose();
    delete _pEntitySystem;
    _pEntitySystem = nullptr;
}

std::default_random_engine* SystemManager::GetRandomEngine() const
{
    return _pRandomEngine; // 返回随机数引擎
}

MessageSystem* SystemManager::GetMessageSystem() const
{
    return _pMessageSystem; // 返回消息系统
}

EntitySystem* SystemManager::GetEntitySystem() const
{
    return _pEntitySystem; // 返回实体系统
}
