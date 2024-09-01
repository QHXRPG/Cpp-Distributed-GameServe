#include "component_collections.h"
#include "log4_help.h"
#include "network.h"

// 构造函数，初始化组件集合的名称
ComponentCollections::ComponentCollections(std::string componentName)
{
    _componentName = componentName;
}

// 添加组件对象到待增加池
void ComponentCollections::Add(IComponent* pObj)
{
    if (_objs.find(pObj->GetSN()) != _objs.end() || _addObjs.find(pObj->GetSN()) != _addObjs.end())
    {
        LOG_ERROR("add component error. Repeat SN. type:" << pObj->GetTypeName() << " sn:" << pObj->GetSN() << " type:" << this->GetClassType().c_str());
        return;
    }

    _addObjs[pObj->GetSN()] = pObj;
}

// 根据序列号获取组件对象
IComponent* ComponentCollections::Get(const uint64 sn)
{
    // 如果传入的序列号为0
    if (sn == 0)
    {
        // 如果组件池中有组件，返回第一个组件
        if (_objs.size() > 0)
            return _objs.begin()->second;

        // 如果待增加的组件池中有组件，返回第一个组件
        if (_addObjs.size() > 0)
            return _addObjs.begin()->second;
    }
    else
    {
        // 在组件池中查找指定序列号的组件
        const auto iter1 = _objs.find(sn);
        if (iter1 != _objs.end())
            return iter1->second;

        // 在待增加的组件池中查找指定序列号的组件
        const auto iter2 = _addObjs.find(sn);
        if (iter2 != _addObjs.end())
            return iter2->second;
    }
    // 如果没有找到，返回nullptr
    return nullptr;
}


// 将组件对象标记为待移除
void ComponentCollections::Remove(uint64 sn)
{
    _removeObjs.push_back(sn);
}

// 获取所有组件对象的映射
std::map<uint64, IComponent*>& ComponentCollections::GetAll()
{
    return _objs;
}

// 交换待增加和待移除的组件对象
void ComponentCollections::Swap()
{
    // 如果待增加的组件池中有组件
    if (_addObjs.size() > 0)
    {
        // 将待增加的组件添加到组件池中
        for (auto pair : _addObjs)
        {
            _objs.insert(std::make_pair(pair.first, pair.second));
        }
        _addObjs.clear();   // 清空待增加的组件池
    }

    // 如果待移除的组件池中有组件
    if (_removeObjs.size() > 0)
    {
        // 遍历待移除的组件池
        for (auto sn : _removeObjs)
        {
            const auto iter = _objs.find(sn);           // 在组件池中查找需要移除的组件
            if (iter != _objs.end())
            {
#if LOG_SYSOBJ_OPEN
                // 记录日志，表示正在释放对象
                LOG_SYSOBJ("*[sys] dispose obj. obj sn:" << iter->second->GetSN() << " type:" << iter->second->GetTypeName() << " thead id:" << std::this_thread::get_id());
#endif
                
                iter->second->ComponentBackToPool();    // 将组件返回到对象池中
                _objs.erase(iter);                      // 从组件池中删除该组件
                continue;   
            }

            // 如果组件池中没有找到需要移除的组件，记录警告日志
            LOG_WARN("remove obj failed. sn:" << sn << " type:" << this->GetClassType().c_str());
        }
        _removeObjs.clear();    // 清空待移除的组件池
    }
}


// 实现IDisposable接口，释放资源
void ComponentCollections::Dispose()
{
    for (auto pair : _objs)
    {
        auto pComponent = pair.second;
        if (pComponent->GetSN() == 0)
            continue;

        pComponent->ComponentBackToPool();
    }

    _objs.clear();
    for (auto pair : _addObjs)
    {
        auto pComponent = pair.second;
        if (pComponent->GetSN() == 0)
            continue;

        pComponent->ComponentBackToPool();
    }

    _addObjs.clear();
    _removeObjs.clear();
}

// 获取组件集合的类型名称
std::string ComponentCollections::GetClassType() const
{
    return _componentName;
}
