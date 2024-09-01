#pragma once

class IComponent;

class IDynamicObjectPool
{
public:
    virtual ~IDynamicObjectPool() = default;          // 析构
    virtual void Update() = 0;                        // 更新
    virtual void DestroyInstance() = 0;               // 销毁实例
    virtual void FreeObject(IComponent* pObj) = 0;    // 销毁 Component
};

