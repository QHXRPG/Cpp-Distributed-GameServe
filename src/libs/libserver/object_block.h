#pragma once
#include "sn_object.h"
#include "disposable.h"

class IDynamicObjectPool;


// 对象池中所有的对象都继承该类
class ObjectBlock :virtual public SnObject, virtual public IDisposable
{
public:
    ObjectBlock(IDynamicObjectPool* pPool);
    virtual ~ObjectBlock();
	virtual void Dispose() override;
    virtual void BackToPool() = 0;  // 清空对象的内存数据

protected:
    IDynamicObjectPool* _pPool{ nullptr };
};