#pragma once
#include "component.h"
#include "global.h"
#include "cache_swap.h"
#include "disposable.h"
#include "component_factory.h"
#include "object_pool.h"
#include "component_collections.h"
#include "system_manager.h"
#include "log4_help.h"

class Packet;

// EntitySystem 类用于管理实体系统中的组件，支持组件的添加、移除、获取等操作，并实现了IDisposable接口。
class EntitySystem : public IDisposable
{
public:
    friend class ConsoleThreadComponent;

    EntitySystem(SystemManager* pMgr);                                      // 构造函数，初始化系统管理器
    virtual ~EntitySystem();                                                // 析构函数

    template <class T, typename... TArgs>
    T* AddComponent(TArgs... args);                                         // 添加组件

    template <typename... TArgs>
    IComponent* AddComponentByName(std::string className, TArgs... args);   // 通过类名添加组件

    template <class T>
    T* GetComponent();                                                      // 获取组件

    void RemoveComponent(IComponent* pObj);                                 // 移除组件

    template<class T>
    ComponentCollections* GetComponentCollections();                        // 获取组件集合

    void Update();                                                          // 更新组件集合
    void Dispose() override;                                                // 实现IDisposable接口，释放资源

private:
    template <class T>
    void AddComponent(T* pComponent);                                       // 添加组件到组件集合
    std::map<uint64, ComponentCollections*> _objSystems;                    // 所有对象的映射
    SystemManager* _systemManager;                                          // 系统管理器
};

// 添加组件到组件集合
template<class T>
inline void EntitySystem::AddComponent(T* pComponent)
{
    const auto typeHashCode = pComponent->GetTypeHashCode();

#if LOG_SYSOBJ_OPEN
    LOG_SYSOBJ("*[sys] add obj. obj sn:" << pComponent->GetSN() << " type:" << pComponent->GetTypeName() << " thead id:" << std::this_thread::get_id());
#endif

    auto iter = _objSystems.find(typeHashCode);
    if (iter == _objSystems.end())
    {
        _objSystems[typeHashCode] = new ComponentCollections(pComponent->GetTypeName());
    }

    auto pEntities = _objSystems[typeHashCode];
    pEntities->Add(dynamic_cast<IComponent*>(pComponent));
}

// 添加组件
template <class T, typename ... TArgs>
T* EntitySystem::AddComponent(TArgs... args)
{
    auto pComponent = DynamicObjectPool<T>::GetInstance()->MallocObject(_systemManager, std::forward<TArgs>(args)...);
    AddComponent(pComponent);
    return pComponent;
}

// 通过类名添加组件
template<typename ...TArgs>
inline IComponent* EntitySystem::AddComponentByName(std::string className, TArgs ...args)
{
    auto pComponent = ComponentFactory<TArgs...>::GetInstance()->Create(_systemManager, className, std::forward<TArgs>(args)...);
    if (pComponent == nullptr)
        return nullptr;

    AddComponent(pComponent);
    return pComponent;
}

// 获取组件
template <class T>
T* EntitySystem::GetComponent()
{
    const auto typeHashCode = typeid(T).hash_code();
    auto iter = _objSystems.find(typeHashCode);
    if (iter == _objSystems.end())
        return nullptr;

    return dynamic_cast<T*>(iter->second->Get()); // 将类型转为所需的类型
}

// 获取组件集合
template<class T>
inline ComponentCollections* EntitySystem::GetComponentCollections()
{
    const auto typeHashCode = typeid(T).hash_code(); // 获取类型名
    auto iter = _objSystems.find(typeHashCode); // 根据类型名在组件池找对应的组件集合
    if (iter == _objSystems.end())
    {
        return nullptr;
    }

    return iter->second;
}
