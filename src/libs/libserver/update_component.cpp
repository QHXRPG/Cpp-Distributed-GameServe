#include "update_component.h"

// 从对象池唤醒时的初始化操作
void UpdateComponent::AwakeFromPool()
{
}

// 返回对象池时的清理操作
void UpdateComponent::BackToPool()
{
    UpdataFunction = nullptr; // 清除更新函数
}
