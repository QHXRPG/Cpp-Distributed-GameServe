#include "system_manager.h"

#include "create_component.h"
#include "message_system.h"
#include "entity_system.h"
#include "update_system.h"
#include "console_thread_component.h"

#include <thread>

SystemManager::SystemManager()
{
    _pEntitySystem = new EntitySystem(this);    // ��ʼ��ʵ��ϵͳ
    _pMessageSystem = new MessageSystem(this);  // ��ʼ����Ϣϵͳ
    _systems.emplace_back(new UpdateSystem());  // ��Ӹ���ϵͳ

    // �����߳�ID�����������
    std::stringstream strStream;
    strStream << std::this_thread::get_id();
    std::string idstr = strStream.str();
    std::seed_seq seed1(idstr.begin(), idstr.end());
    std::minstd_rand0 generator(seed1);
    _pRandomEngine = new std::default_random_engine(generator());
}

void SystemManager::InitComponent()
{
    _pEntitySystem->AddComponent<CreateComponentC>();       // ��Ӵ������
    _pEntitySystem->AddComponent<ConsoleThreadComponent>(); // ��ӿ���̨�߳����
}

void SystemManager::Update()
{
    _pEntitySystem->Update();                   // ����ʵ��ϵͳ
    _pMessageSystem->Update(_pEntitySystem);    // ������Ϣϵͳ

    // ��������ϵͳ
    for (auto iter = _systems.begin(); iter != _systems.end(); ++iter)
    {
        (*iter)->Update(_pEntitySystem);
    }
}

void SystemManager::Dispose()
{
    // �ͷ�����ϵͳ
    for (auto one : _systems)
    {
        one->Dispose();
        delete one;
    }
    _systems.clear();

    // �ͷ����������
    delete _pRandomEngine;
    _pRandomEngine = nullptr;

    // �ͷ���Ϣϵͳ
    _pMessageSystem->Dispose();
    delete _pMessageSystem;
    _pMessageSystem = nullptr;

    // �ͷ�ʵ��ϵͳ
    _pEntitySystem->Dispose();
    delete _pEntitySystem;
    _pEntitySystem = nullptr;
}

std::default_random_engine* SystemManager::GetRandomEngine() const
{
    return _pRandomEngine; // �������������
}

MessageSystem* SystemManager::GetMessageSystem() const
{
    return _pMessageSystem; // ������Ϣϵͳ
}

EntitySystem* SystemManager::GetEntitySystem() const
{
    return _pEntitySystem; // ����ʵ��ϵͳ
}
