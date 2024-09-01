#include "component_collections.h"
#include "log4_help.h"
#include "network.h"

// ���캯������ʼ��������ϵ�����
ComponentCollections::ComponentCollections(std::string componentName)
{
    _componentName = componentName;
}

// ���������󵽴����ӳ�
void ComponentCollections::Add(IComponent* pObj)
{
    if (_objs.find(pObj->GetSN()) != _objs.end() || _addObjs.find(pObj->GetSN()) != _addObjs.end())
    {
        LOG_ERROR("add component error. Repeat SN. type:" << pObj->GetTypeName() << " sn:" << pObj->GetSN() << " type:" << this->GetClassType().c_str());
        return;
    }

    _addObjs[pObj->GetSN()] = pObj;
}

// �������кŻ�ȡ�������
IComponent* ComponentCollections::Get(const uint64 sn)
{
    // �����������к�Ϊ0
    if (sn == 0)
    {
        // ��������������������ص�һ�����
        if (_objs.size() > 0)
            return _objs.begin()->second;

        // ��������ӵ������������������ص�һ�����
        if (_addObjs.size() > 0)
            return _addObjs.begin()->second;
    }
    else
    {
        // ��������в���ָ�����кŵ����
        const auto iter1 = _objs.find(sn);
        if (iter1 != _objs.end())
            return iter1->second;

        // �ڴ����ӵ�������в���ָ�����кŵ����
        const auto iter2 = _addObjs.find(sn);
        if (iter2 != _addObjs.end())
            return iter2->second;
    }
    // ���û���ҵ�������nullptr
    return nullptr;
}


// �����������Ϊ���Ƴ�
void ComponentCollections::Remove(uint64 sn)
{
    _removeObjs.push_back(sn);
}

// ��ȡ������������ӳ��
std::map<uint64, IComponent*>& ComponentCollections::GetAll()
{
    return _objs;
}

// ���������Ӻʹ��Ƴ����������
void ComponentCollections::Swap()
{
    // ��������ӵ�������������
    if (_addObjs.size() > 0)
    {
        // �������ӵ������ӵ��������
        for (auto pair : _addObjs)
        {
            _objs.insert(std::make_pair(pair.first, pair.second));
        }
        _addObjs.clear();   // ��մ����ӵ������
    }

    // ������Ƴ���������������
    if (_removeObjs.size() > 0)
    {
        // �������Ƴ��������
        for (auto sn : _removeObjs)
        {
            const auto iter = _objs.find(sn);           // ��������в�����Ҫ�Ƴ������
            if (iter != _objs.end())
            {
#if LOG_SYSOBJ_OPEN
                // ��¼��־����ʾ�����ͷŶ���
                LOG_SYSOBJ("*[sys] dispose obj. obj sn:" << iter->second->GetSN() << " type:" << iter->second->GetTypeName() << " thead id:" << std::this_thread::get_id());
#endif
                
                iter->second->ComponentBackToPool();    // ��������ص��������
                _objs.erase(iter);                      // ���������ɾ�������
                continue;   
            }

            // ����������û���ҵ���Ҫ�Ƴ����������¼������־
            LOG_WARN("remove obj failed. sn:" << sn << " type:" << this->GetClassType().c_str());
        }
        _removeObjs.clear();    // ��մ��Ƴ��������
    }
}


// ʵ��IDisposable�ӿڣ��ͷ���Դ
void ComponentCollections::Dispose()
{
    for (auto pair : _objs)
    {
        auto pComponent = pair.second;
        if (pComponent->GetSN() == 0)
            continue;

        pComponent->ComponentBackToPool();
    }

    _objs.clear();
    for (auto pair : _addObjs)
    {
        auto pComponent = pair.second;
        if (pComponent->GetSN() == 0)
            continue;

        pComponent->ComponentBackToPool();
    }

    _addObjs.clear();
    _removeObjs.clear();
}

// ��ȡ������ϵ���������
std::string ComponentCollections::GetClassType() const
{
    return _componentName;
}
