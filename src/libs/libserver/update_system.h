#pragma once

#include "system.h"

// ����ϵͳ1
// UpdateSystem��һ��ϵͳ�࣬���ڸ���ʵ��ϵͳ�е�UpdateComponent�����
class UpdateSystem : virtual public ISystem
{
public:
    void Update(EntitySystem* pEntities) override;  // ����ʵ��ϵͳ�е����
};
