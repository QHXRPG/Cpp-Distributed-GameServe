#include "update_system.h"
#include "entity.h"
#include "update_component.h"

void UpdateSystem::Update(EntitySystem* pEntities)
{
	auto pCollections = pEntities->GetComponentCollections<UpdateComponent>();
	if (pCollections == nullptr)
		return;

	pCollections->Swap();                   // 更新组件集合的状态

	auto lists = pCollections->GetAll();    // 在这个组件集合中拿到取出所有组件

	// 遍历每个组件做相应的更新动作
	for (const auto one : lists)
	{
		const auto pComponent = one.second;
		const auto pUpdateComponent = static_cast<UpdateComponent*>(pComponent);
		pUpdateComponent->UpdataFunction();
	}
}
