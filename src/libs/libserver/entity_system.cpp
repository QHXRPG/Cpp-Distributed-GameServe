#include "entity_system.h"
#include "log4.h"
#include "console.h"

// 构造函数，初始化系统管理器
EntitySystem::EntitySystem(SystemManager* pMgr)
{
    _systemManager = pMgr;
}

// 析构函数
EntitySystem::~EntitySystem()
{
}

// 移除组件
void EntitySystem::RemoveComponent(IComponent* pObj)
{
    const auto entitySn = pObj->GetSN();

    const auto typeHashCode = pObj->GetTypeHashCode();
    auto iterObj = _objSystems.find(typeHashCode);
    if (iterObj == _objSystems.end())
    {
        LOG_WARN("destroy class failed. class's name:" << pObj->GetTypeName() << " . not found class.");
        return;
    }

    iterObj->second->Remove(entitySn);

#if LOG_SYSOBJ_OPEN
    LOG_SYSOBJ("*[sys] remove obj. obj sn:" << pObj->GetSN() << " type:" << pObj->GetTypeName() << " thead id:" << std::this_thread::get_id());
#endif
}

// 更新组件集合
void EntitySystem::Update()
{
    for (auto iter : _objSystems)
    {
        iter.second->Swap();
    }
}

// 实现IDisposable接口，释放资源
void EntitySystem::Dispose()
{
    std::set<uint64> baseClass;
    //baseClass.insert(typeid(TimeHeapComponent).hash_code());
    baseClass.insert(typeid(Log4).hash_code());
    baseClass.insert(typeid(Console).hash_code());

    for (auto iter : _objSystems)
    {
        if (baseClass.find(iter.first) != baseClass.end())
            continue;

        iter.second->Dispose();
        delete iter.second;
    }

    _objSystems.clear();
}
