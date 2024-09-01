#include "update_system.h"
#include "entity.h"
#include "update_component.h"

// ����ʵ��ϵͳ�е�UpdateComponent���
void UpdateSystem::Update(EntitySystem* pEntities)
{
    auto pCollections = pEntities->GetComponentCollections<UpdateComponent>();
    if (pCollections == nullptr)
        return;

    pCollections->Swap(); 					// ����������ϵ�״̬
    auto lists = pCollections->GetAll(); 	// ��ȡ�������

    // ����ÿ���������Ӧ�ĸ��¶���
    for (const auto one : lists)
    {
        const auto pComponent = one.second;
        const auto pUpdateComponent = static_cast<UpdateComponent*>(pComponent);
        pUpdateComponent->UpdataFunction();
    }
}
