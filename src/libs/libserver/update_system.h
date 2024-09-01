#pragma once

#include "system.h"

// 更新系统1
// UpdateSystem是一个系统类，用于更新实体系统中的UpdateComponent组件。
class UpdateSystem : virtual public ISystem
{
public:
    void Update(EntitySystem* pEntities) override;  // 更新实体系统中的组件
};
