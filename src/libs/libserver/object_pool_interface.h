#pragma once

class IComponent;

class IDynamicObjectPool
{
public:
    virtual ~IDynamicObjectPool() = default;          // ����
    virtual void Update() = 0;                        // ����
    virtual void DestroyInstance() = 0;               // ����ʵ��
    virtual void FreeObject(IComponent* pObj) = 0;    // ���� Component
};

