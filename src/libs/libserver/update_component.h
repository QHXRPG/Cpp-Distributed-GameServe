#pragma once
#include "component.h"
#include "system.h"
#include <functional>

// UpdateComponent����һ������࣬����һ�����º����������ڸ���ϵͳ��ִ���ض��ĸ��²�����
class UpdateComponent : public Component<UpdateComponent>, public IAwakeFromPoolSystem<>
{
public:
    void AwakeFromPool() override; // �Ӷ���ػ���
    void BackToPool() override;    // ���ض����

    std::function<void()> UpdataFunction{ nullptr }; // ���º���
};
