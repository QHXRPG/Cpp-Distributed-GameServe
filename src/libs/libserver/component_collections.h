#pragma once
#include "component.h"
#include "disposable.h"

#include <list>
#include <map>

// ComponentCollections �ǰ�������������
// ComponentCollections �����ڹ���ͬ���͵�������󼯺ϣ�֧���������ӡ��Ƴ�����ȡ�Ȳ�������ʵ����IDisposable�ӿڡ�
class ComponentCollections : public IDisposable
{
public:
    ComponentCollections(std::string componentName);  // ���캯������ʼ��������ϵ�����
    void Add(IComponent* pObj);                       // ���������󵽴����ӳ�
    void Remove(uint64 sn);                           // �����������Ϊ���Ƴ�
    IComponent* Get(uint64 sn = 0);                   // �������кŻ�ȡ�������
    std::map<uint64, IComponent*>& GetAll();          // ��ȡ������������ӳ��
    void Swap();                                      // ���������Ӻʹ��Ƴ����������
    void Dispose() override;                          // ʵ��IDisposable�ӿڣ��ͷ���Դ
    std::string GetClassType() const;                 // ��ȡ������ϵ���������

private:    
    std::map<uint64, IComponent*> _objs;    // �����
    std::map<uint64, IComponent*> _addObjs; // �����ӵ������
    std::list<uint64> _removeObjs;          // ���Ƴ��������
    std::string _componentName;             // ������ϵ�����
};
