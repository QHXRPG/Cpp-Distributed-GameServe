#pragma once
#include "sn_object.h"
#include "disposable.h"

class IDynamicObjectPool;


// ����������еĶ��󶼼̳и���
class ObjectBlock :virtual public SnObject, virtual public IDisposable
{
public:
    ObjectBlock(IDynamicObjectPool* pPool);
    virtual ~ObjectBlock();
	virtual void Dispose() override;
    virtual void BackToPool() = 0;  // ��ն�����ڴ�����

protected:
    IDynamicObjectPool* _pPool{ nullptr };
};