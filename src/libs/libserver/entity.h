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
������붨����һ��IEntity���Լ�һ��ģ����Entity����Ҫ���ڹ���ʵ�弰�������
IEntity�ṩ����ӡ���ȡ���Ƴ������ģ�庯������Entityģ�������ṩ�˻�ȡ�������ƺ͹�ϣ��Ĺ��ܡ�
ͨ��������ƣ����Է���ع���Ͳ���ʵ�弰��������ٽ������ģ�黯�Ϳ�ά���ԡ�
*/

// IEntity�࣬�̳���IComponent�����ڱ�ʾһ��ʵ�����
class IEntity : public IComponent
{
public:
    virtual ~IEntity() = default; // ��������
    void BackToPool() override; // �����󷵻ص������

    // ������
    template <class T, typename... TArgs>
    T* AddComponent(TArgs... args);

    // ��ȡ���
    template<class T>
    T* GetComponent();

    // �Ƴ����
    template<class T>
    void RemoveComponent();

    // �Ƴ�ָ������� �� EntitySystem ȥ����
    void RemoveComponent(IComponent* pObj);

protected:
    std::map<uint64, IComponent*> _components; // �洢�����ӳ��
};

// ������
template <class T, typename... TArgs>
inline T* IEntity::AddComponent(TArgs... args)
{
    auto pComponent = GetSystemManager()->GetEntitySystem()->AddComponent<T>(std::forward<TArgs>(args)...);
    pComponent->SetParent(this);                                            // Ϊ������ָ����ʵ��Ϊ��ǰ����ʹ��
    _components.insert(std::make_pair(pComponent->GetSN(), pComponent));    // ������ӵ� _components map��   
    return pComponent;
}

// ��ȡ���
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

// �Ƴ����
template<class T>
void IEntity::RemoveComponent()
{
    // ��ɾ�������������
    const auto typeHashCode = typeid(T).hash_code();    // �õ�������ļ�����ϣֵ��
    const auto iter = _components.find(typeHashCode);   // ͨ������ map ��ȡ����Ӧ�����
    if (iter == _components.end())
    {
        LOG_ERROR("Entity RemoveComponent error. not find. className:" << typeid(T).name());
        return;
    }

    auto pComponent = iter->second;
    RemoveComponent(pComponent);
}

// ʵ��ģ���࣬�̳���IEntity
template<class T>
class Entity : public IEntity
{
public:
    const char* GetTypeName() override; // ��ȡ��������
    uint64 GetTypeHashCode() override; // ��ȡ���͹�ϣ��
};

// ��ȡ��������
template <class T>
const char* Entity<T>::GetTypeName()
{
    return typeid(T).name();
}

// ��ȡ���͹�ϣ��
template <class T>
uint64 Entity<T>::GetTypeHashCode()
{
    return typeid(T).hash_code();
}
