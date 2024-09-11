#pragma once
#include <string>
#include <functional>
#include <map>
#include <iostream>
#include <mutex>

// ComponentFactory模板类，用于创建组件
template<typename ...Targs>
class ComponentFactory {
public:
    // 定义工厂函数类型
    typedef std::function<IComponent*(SystemManager*, Targs...)> FactoryFunction;

    // 获取单例实例
    static ComponentFactory<Targs...>* GetInstance() {
        if (_pInstance == nullptr) {
            _pInstance = new ComponentFactory<Targs...>();
        }
        return _pInstance;
    }

    // 注册组件工厂函数
    bool Regist(const std::string& className, FactoryFunction pFunc) {
        std::lock_guard<std::mutex> guard(_lock);
        if (_map.find(className) != _map.end())
            return false;

        _map.insert(std::make_pair(className, pFunc));
        return true;
    }

    // 检查组件是否已注册
    bool IsRegisted(const std::string& className) {
        std::lock_guard<std::mutex> guard(_lock);
        return _map.find(className) != _map.end();
    }

    // 创建组件实例
    IComponent* Create(SystemManager* pSysMgr, const std::string className, Targs... args) {
        _lock.lock();
        auto iter = _map.find(className);
        if (iter == _map.end()) {
            std::cout << "ComponentFactory Create failed. can't find component. className:" << className.c_str() << std::endl;
            _lock.unlock();
            return nullptr;
        }
        auto fun = iter->second;
        _lock.unlock();

        return fun(pSysMgr, std::forward<Targs>(args)...);
    }

private:
    static ComponentFactory<Targs...>* _pInstance;  // 单例实例指针
    std::map<std::string, FactoryFunction> _map;    // 存储类名与工厂函数的映射
    std::mutex _lock;  								// 互斥锁，保护_map的访问
};

// 初始化静态成员变量
template<typename ...Targs>
ComponentFactory<Targs...>* ComponentFactory<Targs...>::_pInstance = nullptr;
