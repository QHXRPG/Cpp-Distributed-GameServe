#pragma once

#include "disposable.h"
#include "system.h"
#include "common.h"

#include <list>
#include <random>

// SystemManager������ MessageSystem �� EntitySystem
// Э���͹������ϵͳ������������ںͲ���
class EntitySystem;
class MessageSystem;

class SystemManager : virtual public IDisposable
{
public:
    SystemManager();                                        // ���캯��
    void InitComponent();                                   // ��ʼ�����
    virtual void Update();                                  // ����ϵͳ
    void Dispose() override;                                // �ͷ���Դ
    MessageSystem* GetMessageSystem() const;                // ��ȡMessageSystem
    EntitySystem* GetEntitySystem() const;                  // ��ȡEntitySystem
    std::default_random_engine* GetRandomEngine() const;    // ��ȡ���������

protected:
    MessageSystem* _pMessageSystem;                 // ��Ϣϵͳָ��
    EntitySystem* _pEntitySystem;                   // ʵ��ϵͳָ��
    std::list<ISystem*> _systems;                   // ϵͳ�б�
    std::default_random_engine* _pRandomEngine;     // ���������ָ��
};
