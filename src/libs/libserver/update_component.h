#pragma once
#include "component.h"
#include "system.h"
#include <functional>

// UpdateComponent类是一个组件类，包含一个更新函数，用于在更新系统中执行特定的更新操作。
class UpdateComponent : public Component<UpdateComponent>, public IAwakeFromPoolSystem<>
{
public:
    void AwakeFromPool() override; // 从对象池唤醒
    void BackToPool() override;    // 返回对象池

    std::function<void()> UpdataFunction{ nullptr }; // 更新函数
};
