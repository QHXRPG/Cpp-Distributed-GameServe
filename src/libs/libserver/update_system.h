#pragma once

#include "system.h"

// ����ϵͳ1
class UpdateSystem : virtual public ISystem
{
public:
	void Update(EntitySystem* pEntities) override;  // 
};
