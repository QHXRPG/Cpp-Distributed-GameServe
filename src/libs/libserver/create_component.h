// Ԥ��ͷ�ļ����ظ�����
#pragma once

// ������Ҫ��ͷ�ļ�
#include "entity.h"
#include "system.h"
#include "message_system.h"

// ǰ������
class Packet;

// CreateComponentC �ඨ�壬�̳��� Entity �� IAwakeFromPoolSystem
class CreateComponentC : public Entity<CreateComponentC>, public IAwakeFromPoolSystem<>
{
public:
    // �Ӷ�����л��ѵķ���
    void AwakeFromPool() override;
    // ���ض���صķ���
    void BackToPool() override;

private:
    // ����������ĺ���
    void HandleCreateComponent(Packet* pPacket) const;
    // �����Ƴ�����ĺ���
    void HandleRemoveComponent(Packet* pPacket);
};