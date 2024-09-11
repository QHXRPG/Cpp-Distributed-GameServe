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

// DynamicObjectPool ģ���࣬�̳��� IDynamicObjectPool
template <typename T>
class DynamicObjectPool : public IDynamicObjectPool {
public:
    // ��ȡ����ʵ��
    static DynamicObjectPool<T>* GetInstance() {
        std::lock_guard<std::mutex> guard(_instanceLock);

        if (_pInstance == nullptr) {
            _pInstance = new DynamicObjectPool<T>();
            DynamicObjectPoolMgr::GetInstance()->AddPool(_pInstance);
        }

        return _pInstance;
    }

    // ���ٵ���ʵ��
    void DestroyInstance() override {
        std::lock_guard<std::mutex> guard(_instanceLock);
        if (_pInstance == nullptr)
            return;

        delete _pInstance;
        _pInstance = nullptr;
    }

    DynamicObjectPool();
    ~DynamicObjectPool();

    // �������
    template<typename ...Targs>
    T* MallocObject(SystemManager* pSysMgr, Targs... args);

    
    void Update() override;                     // ���¶����
    void FreeObject(IComponent* pObj) override; // �ͷŶ���
    void Show();                                // ��ʾ�������Ϣ

private:
    void CreateOne();                           // ����һ������

private:
    std::queue<T*> _free;                       // ���ж������
    std::mutex _freeLock;                       // ���ж�����л�����

    std::mutex _inUseLock;                      // ʹ���ж��󻥳���
    CacheRefresh<T> _objInUse;                  // ʹ���ж��󻺴�

    static DynamicObjectPool<T>* _pInstance;    // ����ʵ��
    static std::mutex _instanceLock;            // ����ʵ��������

#if _DEBUG
    int _totalCall{ 0 };  // ���ô���ͳ�ƣ������ڵ��ԣ�
#endif
};

// ��ʼ����̬��Ա����
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
    // ������֮ǰ������һ��Update����inuse�еĶ���ص�Free��
    Update();

#if _DEBUG
    if (_objInUse.GetReaderCache()->size() > 0) {
        // ����û��ȫ����������һ��������~
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

// �Ӷ������ȡ��һ�����󣬲����б�Ҫ�ĳ�ʼ��
template <typename T>
template <typename ... Targs>
T* DynamicObjectPool<T>::MallocObject(SystemManager* pSysMgr, Targs... args) {
    _freeLock.lock();
    
    // ������ж���Ϊ�գ��򴴽�һ���µĶ���
    if (_free.size() == 0) {
        CreateOne();
        // std::cout << "+ obj. object pool. type:" << typeid(T).name() << std::endl;  // ������Ϣ
    }

#if _DEBUG
    // ����ģʽ�£�ͳ���ܵ��ô���
    _totalCall++;
#endif

    auto pObj = _free.front();                          // �ӿ��ж�����ȡ��һ������
    _freeLock.unlock();                                 // �������ж���
    pObj->ResetSN();                                    // ���ö�������к�
    pObj->SetSystemManager(pSysMgr);                    // ���ö����ϵͳ������
    pObj->AwakeFromPool(std::forward<Targs>(args)...);  // ���ö���� AwakeFromPool �������г�ʼ��
    _inUseLock.lock();                                  // ����ʹ���ж��󻺴棬ȷ���̰߳�ȫ
    _objInUse.GetAddCache()->push_back(pObj);           // ��������ӵ�ʹ���ж������ӻ�����
    _inUseLock.unlock();
    return pObj;
}


// ���¶���أ������Ƴ��Ķ����ƻؿ��ж���
template <typename T>
void DynamicObjectPool<T>::Update() {
    std::list<T*> freeObjs;                         // ���ڴ洢���ƻؿ��ж��еĶ���
    _inUseLock.lock();                              // ����ʹ���ж��󻺴�
    if (_objInUse.CanSwap()) {                      // ����Ƿ��ж�����Ҫ����
        freeObjs = _objInUse.Swap();                // �������棬��ȡ���Ƴ��Ķ���
    }
    _inUseLock.unlock();                            // ����ʹ���ж��󻺴�

    std::lock_guard<std::mutex> guard(_freeLock);   // �������ж���
    for (auto one : freeObjs) {                     // �����Ƴ��Ķ����ƻؿ��ж���
        _free.push(one);
    }
}

// �ͷŶ��󣬽�������Ϊ���Ƴ�
template<typename T>
inline void DynamicObjectPool<T>::FreeObject(IComponent* pObj) {
    std::lock_guard<std::mutex> guard(_inUseLock);                      // ����ʹ���ж��󻺴�
    _objInUse.GetRemoveCache()->emplace_back(dynamic_cast<T*>(pObj));   // ��������ӵ��Ƴ�����
}

// ��ʾ�������Ϣ�������ܶ����������ж�������ʹ���ж�����
template <typename T>
void DynamicObjectPool<T>::Show() {
    std::lock_guard<std::mutex> guard(_freeLock);                                                                               // �������ж���
    std::lock_guard<std::mutex> guardInUse(_inUseLock);                                                                         // ����ʹ���ж��󻺴�
    auto count = _objInUse.GetReaderCache()->size() + _objInUse.GetAddCache()->size() + _objInUse.GetRemoveCache()->size();     // ����ʹ���ж�������

    std::stringstream log;
    log << "*************************** " << "\n";
    log << "pool total count:\t" << _free.size() + count << "\n";       // ����ܶ�����
    log << "free count:\t\t" << _free.size() << "\n";                   // ������ж�����
    log << "in use count:\t" << count << "\n";                          // ���ʹ���ж�����

#if _DEBUG
    log << "total call:\t" << _totalCall << "\n"; 
#endif

    std::cout << log.str() << std::endl; 
}
