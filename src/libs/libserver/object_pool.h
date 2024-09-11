#pragma once
#include <queue>
#include <iostream>
#include <sstream>
#include <list>

#include "sn_object.h"
#include "packet.h"
#include "object_pool_interface.h"
#include "cache_refresh.h"
#include "object_pool_mgr.h"
#include "system_manager.h"

// DynamicObjectPool 模板类，继承自 IDynamicObjectPool
template <typename T>
class DynamicObjectPool : public IDynamicObjectPool {
public:
    // 获取单例实例
    static DynamicObjectPool<T>* GetInstance() {
        std::lock_guard<std::mutex> guard(_instanceLock);

        if (_pInstance == nullptr) {
            _pInstance = new DynamicObjectPool<T>();
            DynamicObjectPoolMgr::GetInstance()->AddPool(_pInstance);
        }

        return _pInstance;
    }

    // 销毁单例实例
    void DestroyInstance() override {
        std::lock_guard<std::mutex> guard(_instanceLock);
        if (_pInstance == nullptr)
            return;

        delete _pInstance;
        _pInstance = nullptr;
    }

    DynamicObjectPool();
    ~DynamicObjectPool();

    // 分配对象
    template<typename ...Targs>
    T* MallocObject(SystemManager* pSysMgr, Targs... args);

    
    void Update() override;                     // 更新对象池
    void FreeObject(IComponent* pObj) override; // 释放对象
    void Show();                                // 显示对象池信息

private:
    void CreateOne();                           // 创建一个对象

private:
    std::queue<T*> _free;                       // 空闲对象队列
    std::mutex _freeLock;                       // 空闲对象队列互斥锁

    std::mutex _inUseLock;                      // 使用中对象互斥锁
    CacheRefresh<T> _objInUse;                  // 使用中对象缓存

    static DynamicObjectPool<T>* _pInstance;    // 单例实例
    static std::mutex _instanceLock;            // 单例实例互斥锁

#if _DEBUG
    int _totalCall{ 0 };  // 调用次数统计（仅用于调试）
#endif
};

// 初始化静态成员变量
template <typename T>
DynamicObjectPool<T>* DynamicObjectPool<T>::_pInstance = nullptr;

template <typename T>
std::mutex DynamicObjectPool<T>::_instanceLock;

template <typename T>
DynamicObjectPool<T>::DynamicObjectPool() {
}

template <typename T>
void DynamicObjectPool<T>::CreateOne() {
    T* pObj = new T();
    pObj->SetPool(this);
    _free.push(pObj);
}

template <typename T>
DynamicObjectPool<T>::~DynamicObjectPool() {
    // 再销毁之前，再做一次Update，让inuse中的对象回到Free中
    Update();

#if _DEBUG
    if (_objInUse.GetReaderCache()->size() > 0) {
        // 数据没有全部还回来，一定有问题~
        std::cout << "dispose object pool error. type:" << typeid(T).name() << " inUse size:" << _objInUse.GetReaderCache()->size() << " free size:" << _free.size() << std::endl;
    }
#endif

    std::cout << "dispose object pool. type:" << typeid(T).name() << std::endl;

    while (_free.size() > 0) {
        auto iter = _free.front();
        delete iter;
        _free.pop();
    }
}

// 从对象池中取出一个对象，并进行必要的初始化
template <typename T>
template <typename ... Targs>
T* DynamicObjectPool<T>::MallocObject(SystemManager* pSysMgr, Targs... args) {
    _freeLock.lock();
    
    // 如果空闲队列为空，则创建一个新的对象
    if (_free.size() == 0) {
        CreateOne();
        // std::cout << "+ obj. object pool. type:" << typeid(T).name() << std::endl;  // 调试信息
    }

#if _DEBUG
    // 调试模式下，统计总调用次数
    _totalCall++;
#endif

    auto pObj = _free.front();                          // 从空闲队列中取出一个对象
    _freeLock.unlock();                                 // 解锁空闲队列
    pObj->ResetSN();                                    // 重置对象的序列号
    pObj->SetSystemManager(pSysMgr);                    // 设置对象的系统管理器
    pObj->AwakeFromPool(std::forward<Targs>(args)...);  // 调用对象的 AwakeFromPool 方法进行初始化
    _inUseLock.lock();                                  // 锁定使用中对象缓存，确保线程安全
    _objInUse.GetAddCache()->push_back(pObj);           // 将对象添加到使用中对象的添加缓存中
    _inUseLock.unlock();
    return pObj;
}


// 更新对象池，将待移除的对象移回空闲队列
template <typename T>
void DynamicObjectPool<T>::Update() {
    std::list<T*> freeObjs;                         // 用于存储待移回空闲队列的对象
    _inUseLock.lock();                              // 锁定使用中对象缓存
    if (_objInUse.CanSwap()) {                      // 检查是否有对象需要交换
        freeObjs = _objInUse.Swap();                // 交换缓存，获取待移除的对象
    }
    _inUseLock.unlock();                            // 解锁使用中对象缓存

    std::lock_guard<std::mutex> guard(_freeLock);   // 锁定空闲队列
    for (auto one : freeObjs) {                     // 将待移除的对象移回空闲队列
        _free.push(one);
    }
}

// 释放对象，将对象标记为待移除
template<typename T>
inline void DynamicObjectPool<T>::FreeObject(IComponent* pObj) {
    std::lock_guard<std::mutex> guard(_inUseLock);                      // 锁定使用中对象缓存
    _objInUse.GetRemoveCache()->emplace_back(dynamic_cast<T*>(pObj));   // 将对象添加到移除缓存
}

// 显示对象池信息，包括总对象数、空闲对象数和使用中对象数
template <typename T>
void DynamicObjectPool<T>::Show() {
    std::lock_guard<std::mutex> guard(_freeLock);                                                                               // 锁定空闲队列
    std::lock_guard<std::mutex> guardInUse(_inUseLock);                                                                         // 锁定使用中对象缓存
    auto count = _objInUse.GetReaderCache()->size() + _objInUse.GetAddCache()->size() + _objInUse.GetRemoveCache()->size();     // 计算使用中对象总数

    std::stringstream log;
    log << "*************************** " << "\n";
    log << "pool total count:\t" << _free.size() + count << "\n";       // 输出总对象数
    log << "free count:\t\t" << _free.size() << "\n";                   // 输出空闲对象数
    log << "in use count:\t" << count << "\n";                          // 输出使用中对象数

#if _DEBUG
    log << "total call:\t" << _totalCall << "\n"; 
#endif

    std::cout << log.str() << std::endl; 
}
