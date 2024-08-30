#pragma once
#include "sn_object.h"
#include "common.h"

class IEntity;
class IDynamicObjectPool;
class SystemManager;


/*
������붨����һ��IComponent���Լ�һ��ģ����Component����Ҫ���ڹ���������丸����
IComponent�ṩ�����ö���ء��������ϵͳ�������ĺ������Լ���ȡ�������ϵͳ�������ĺ�����
ͨ��������ƣ����Է���ع���Ͳ���������丸���󣬴ٽ������ģ�黯�Ϳ�ά���ԡ�
���ģ����Component���ṩ�˻�ȡ�������ƺ͹�ϣ��Ĺ���
*/

// IComponent�࣬�̳���SnObject�����ڱ�ʾһ���������
class IComponent : virtual public SnObject
{
public:
    friend class EntitySystem;

    virtual ~IComponent() = default;            // ��������

    void SetPool(IDynamicObjectPool* pPool);    // ���ö����
    void SetParent(IEntity* pObj);              // ���ø�����
    void SetSystemManager(SystemManager* pObj); // ����ϵͳ������

    bool IsActive() const { return _active; }   // �ж�����Ƿ񼤻�

    // ��ȡ������ģ�庯��
    template<class T>
    T* GetParent();

    IEntity* GetParent() const;                 // ��ȡ������

    SystemManager* GetSystemManager() const;    // ��ȡϵͳ������
    virtual void BackToPool() = 0;              // �����󷵻ص������
    virtual void ComponentBackToPool();         // ��������ص������

    virtual const char* GetTypeName() = 0;      // ��ȡ��������
    virtual uint64 GetTypeHashCode() = 0;       // ��ȡ���͹�ϣ��

protected:
    bool _active{ true };                       // �������״̬

private:
    IEntity* _parent{ nullptr };                // ��ʵ��ָ�룬�����������ʵ�� 
    SystemManager* _pSystemManager{ nullptr };  // ϵͳ������ָ��
    IDynamicObjectPool* _pPool{ nullptr };      // �����ָ��
};

// ��ȡ������ʵ��
template <class T>
T* IComponent::GetParent()
{
    return dynamic_cast<T*>(_parent);
}

// Componentģ���࣬�̳���IComponent
template<class T>
class Component : public IComponent
{
public:
    const char* GetTypeName() override;         // ��ȡ��������
    uint64 GetTypeHashCode() override;          // ��ȡ���͹�ϣ��
};

// ��ȡ��������ʵ��
template <class T>
const char* Component<T>::GetTypeName()
{
    return typeid(T).name();
}

// ��ȡ���͹�ϣ��ʵ��
template <class T>
uint64 Component<T>::GetTypeHashCode()
{
    return typeid(T).hash_code();
}
