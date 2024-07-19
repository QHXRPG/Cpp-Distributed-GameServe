#include "entity.h"
#include "entity_system.h"

void IEntity::BackToPool() {
    for (auto pair : _components)
    {
        // �� EntitySystem ȥ����
        const auto pEntitySystem = GetSystemManager()->GetEntitySystem();
        if (pEntitySystem != nullptr)
            pEntitySystem->RemoveComponent(pair.second);
        else
            pair.second->ComponentBackToPool();
    }

    _components.clear();
}

void IEntity::RemoveComponent(IComponent* pComponent)
{
    const auto typeHashCode = pComponent->GetTypeHashCode();
    _components.erase(typeHashCode);

    const auto pEntitySystem = GetSystemManager()->GetEntitySystem();
    if (pEntitySystem == nullptr)
    {
        pComponent->ComponentBackToPool();
    }
    else
    {
        // �� EntitySystem ȥ����
        pEntitySystem->RemoveComponent(pComponent);
    }
}
