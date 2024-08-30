#pragma once

#include <list>
#include <mutex>

#include "singleton.h"
#include "disposable.h"

#include "object_pool_interface.h"

/*
��̬����ع�������ʵ���˵���ģʽ�Ϳ����ٽӿڡ�
�ṩ����Ӷ����AddPool�����¶����Update�����ٶ����Dispose�Ĺ��ܡ�
*/

class DynamicObjectPoolMgr :public Singleton<DynamicObjectPoolMgr>, public IDisposable
{
public:
	void AddPool(IDynamicObjectPool* pPool);
	void Update();

	void Dispose() override;

private:
	std::mutex _lock;
	std::list<IDynamicObjectPool*> _pools;
};
