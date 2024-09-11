#pragma once
#include <string>
#include <functional>
#include <map>
#include <iostream>
#include <mutex>

// ComponentFactoryģ���࣬���ڴ������
template<typename ...Targs>
class ComponentFactory {
public:
    // ���幤����������
    typedef std::function<IComponent*(SystemManager*, Targs...)> FactoryFunction;

    // ��ȡ����ʵ��
    static ComponentFactory<Targs...>* GetInstance() {
        if (_pInstance == nullptr) {
            _pInstance = new ComponentFactory<Targs...>();
        }
        return _pInstance;
    }

    // ע�������������
    bool Regist(const std::string& className, FactoryFunction pFunc) {
        std::lock_guard<std::mutex> guard(_lock);
        if (_map.find(className) != _map.end())
            return false;

        _map.insert(std::make_pair(className, pFunc));
        return true;
    }

    // �������Ƿ���ע��
    bool IsRegisted(const std::string& className) {
        std::lock_guard<std::mutex> guard(_lock);
        return _map.find(className) != _map.end();
    }

    // �������ʵ��
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
    static ComponentFactory<Targs...>* _pInstance;  // ����ʵ��ָ��
    std::map<std::string, FactoryFunction> _map;    // �洢�����빤��������ӳ��
    std::mutex _lock;  								// ������������_map�ķ���
};

// ��ʼ����̬��Ա����
template<typename ...Targs>
ComponentFactory<Targs...>* ComponentFactory<Targs...>::_pInstance = nullptr;
