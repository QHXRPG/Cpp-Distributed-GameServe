#pragma once
#include "sn_object.h"
#include "common.h"

class IEntity;
class IDynamicObjectPool;
class SystemManager;


/*
这个代码定义了一个IComponent类以及一个模板类Component，主要用于管理组件及其父对象。
IComponent提供了设置对象池、父对象和系统管理器的函数，以及获取父对象和系统管理器的函数。
通过这种设计，可以方便地管理和操作组件及其父对象，促进代码的模块化和可维护性。
组件模板类Component则提供了获取类型名称和哈希码的功能
*/

// IComponent类，继承自SnObject，用于表示一个组件对象
class IComponent : virtual public SnObject
{
public:
    friend class EntitySystem;

    virtual ~IComponent() = default;            // 析构函数

    void SetPool(IDynamicObjectPool* pPool);    // 设置对象池
    void SetParent(IEntity* pObj);              // 设置父对象
    void SetSystemManager(SystemManager* pObj); // 设置系统管理器

    bool IsActive() const { return _active; }   // 判断组件是否激活

    // 获取父对象模板函数
    template<class T>
    T* GetParent();

    IEntity* GetParent() const;                 // 获取父对象

    SystemManager* GetSystemManager() const;    // 获取系统管理器
    virtual void BackToPool() = 0;              // 将对象返回到对象池
    virtual void ComponentBackToPool();         // 将组件返回到对象池

    virtual const char* GetTypeName() = 0;      // 获取类型名称
    virtual uint64 GetTypeHashCode() = 0;       // 获取类型哈希码

protected:
    bool _active{ true };                       // 组件激活状态

private:
    IEntity* _parent{ nullptr };                // 父实体指针，该组件所属的实体 
    SystemManager* _pSystemManager{ nullptr };  // 系统管理器指针
    IDynamicObjectPool* _pPool{ nullptr };      // 对象池指针
};

// 获取父对象实现
template <class T>
T* IComponent::GetParent()
{
    return dynamic_cast<T*>(_parent);
}

// Component模板类，继承自IComponent
template<class T>
class Component : public IComponent
{
public:
    const char* GetTypeName() override;         // 获取类型名称
    uint64 GetTypeHashCode() override;          // 获取类型哈希码
};

// 获取类型名称实现
template <class T>
const char* Component<T>::GetTypeName()
{
    return typeid(T).name();
}

// 获取类型哈希码实现
template <class T>
uint64 Component<T>::GetTypeHashCode()
{
    return typeid(T).hash_code();
}
