#pragma once

#include <list>
#include <mutex>

#include "singleton.h"
#include "disposable.h"

#include "object_pool_interface.h"

/*
动态对象池管理器，实现了单例模式和可销毁接口。
提供了添加对象池AddPool、更新对象池Update和销毁对象池Dispose的功能。
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
