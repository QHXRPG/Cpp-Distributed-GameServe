#include "update_component.h"

// �Ӷ���ػ���ʱ�ĳ�ʼ������
void UpdateComponent::AwakeFromPool()
{
}

// ���ض����ʱ���������
void UpdateComponent::BackToPool()
{
    UpdataFunction = nullptr; // ������º���
}
