#pragma once

#include "system.h"

// 更新系统1
class UpdateSystem : virtual public ISystem
{
public:
	void Update(EntitySystem* pEntities) override;  // 
};
