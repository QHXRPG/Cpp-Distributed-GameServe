#pragma once
#include "component.h"
#include "disposable.h"

#include <list>
#include <map>

// ComponentCollections 是按组件类型来存的
// ComponentCollections 类用于管理不同类型的组件对象集合，支持组件的添加、移除、获取等操作，并实现了IDisposable接口。
class ComponentCollections : public IDisposable
{
public:
    ComponentCollections(std::string componentName);  // 构造函数，初始化组件集合的名称
    void Add(IComponent* pObj);                       // 添加组件对象到待增加池
    void Remove(uint64 sn);                           // 将组件对象标记为待移除
    IComponent* Get(uint64 sn = 0);                   // 根据序列号获取组件对象
    std::map<uint64, IComponent*>& GetAll();          // 获取所有组件对象的映射
    void Swap();                                      // 交换待增加和待移除的组件对象
    void Dispose() override;                          // 实现IDisposable接口，释放资源
    std::string GetClassType() const;                 // 获取组件集合的类型名称

private:    
    std::map<uint64, IComponent*> _objs;    // 组件池
    std::map<uint64, IComponent*> _addObjs; // 待增加的组件池
    std::list<uint64> _removeObjs;          // 待移除的组件池
    std::string _componentName;             // 组件集合的名称
};
