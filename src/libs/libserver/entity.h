#pragma once

#include <map>
#include <list>
#include <memory>
#include <algorithm>
#include <queue>

#include "component.h"
#include "object_pool.h"
#include "entity_system.h"

/*
这个代码定义了一个IEntity类以及一个模板类Entity，主要用于管理实体及其组件。
IEntity提供了添加、获取和移除组件的模板函数，而Entity模板类则提供了获取类型名称和哈希码的功能。
通过这种设计，可以方便地管理和操作实体及其组件，促进代码的模块化和可维护性。
*/

// IEntity类，继承自IComponent，用于表示一个实体对象
class IEntity : public IComponent
{
public:
    virtual ~IEntity() = default; // 析构函数
    void BackToPool() override; // 将对象返回到对象池

    // 添加组件
    template <class T, typename... TArgs>
    T* AddComponent(TArgs... args);

    // 获取组件
    template<class T>
    T* GetComponent();

    // 移除组件
    template<class T>
    void RemoveComponent();

    // 移除指定组件， 由 EntitySystem 去销毁
    void RemoveComponent(IComponent* pObj);

protected:
    std::map<uint64, IComponent*> _components; // 存储组件的映射
};

// 添加组件
template <class T, typename... TArgs>
inline T* IEntity::AddComponent(TArgs... args)
{
    auto pComponent = GetSystemManager()->GetEntitySystem()->AddComponent<T>(std::forward<TArgs>(args)...);
    pComponent->SetParent(this);                                            // 为这个组件指定父实体为当前对象，使得
    _components.insert(std::make_pair(pComponent->GetSN(), pComponent));    // 把组件加到 _components map中   
    return pComponent;
}

// 获取组件
template<class T>
T* IEntity::GetComponent()
{
    auto iter = std::find_if(_components.begin(), _components.end(), [](auto pair)
        {
            if (dynamic_cast<T*>(pair.second) != nullptr)
                return true;

            return false;
        });
    if (iter == _components.end())
        return nullptr;

    return dynamic_cast<T*>(iter->second);
}

// 移除组件
template<class T>
void IEntity::RemoveComponent()
{
    // 先删除本地组件数据
    const auto typeHashCode = typeid(T).hash_code();    // 拿到该组件的键（哈希值）
    const auto iter = _components.find(typeHashCode);   // 通过键从 map 中取出对应的组件
    if (iter == _components.end())
    {
        LOG_ERROR("Entity RemoveComponent error. not find. className:" << typeid(T).name());
        return;
    }

    auto pComponent = iter->second;
    RemoveComponent(pComponent);
}

// 实体模板类，继承自IEntity
template<class T>
class Entity : public IEntity
{
public:
    const char* GetTypeName() override; // 获取类型名称
    uint64 GetTypeHashCode() override; // 获取类型哈希码
};

// 获取类型名称
template <class T>
const char* Entity<T>::GetTypeName()
{
    return typeid(T).name();
}

// 获取类型哈希码
template <class T>
uint64 Entity<T>::GetTypeHashCode()
{
    return typeid(T).hash_code();
}
