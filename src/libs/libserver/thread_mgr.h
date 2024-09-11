#pragma once

#include <mutex>
#include <vector>

#include "common.h"
#include "thread.h"
#include "cache_swap.h"
#include "singleton.h"
#include "entity_system.h"
#include "component_factory.h"
#include "regist_to_factory.h"
#include "message_system_help.h"

class Packet;

// ThreadMgr��̳��� Singleton �� SystemManager
// �ṩ�����������̡߳����¡����ֹͣ״̬���ͷ���Դ�ʹ����̵߳ķ�����
class ThreadMgr : public Singleton<ThreadMgr>, public SystemManager {
public:
    ThreadMgr();                                // ���캯��
    void StartAllThread();                      // ���������߳�
    void Update() override;                     // ��д�ĸ��·���
    bool IsStopAll();                           // ����Ƿ�ֹͣ�����߳�
    bool IsDisposeAll();                        // ����Ƿ��ͷ�������Դ
    void Dispose() override;                    // ��д���ͷ���Դ����
    void CreateThread();                        // �����߳�

    // �������
    template<class T, typename ...TArgs>
    void CreateComponent(TArgs... args);

    void DispatchPacket(Packet* pPacket);       // �ַ���Ϣ��

private:
    // �������������ͣ�
    template <typename...Args>
    void AnalyseParam(Proto::CreateComponent& proto, int value, Args...args);
    
    // �����������ַ�����
    template <typename...Args>
    void AnalyseParam(Proto::CreateComponent& proto, std::string value, Args...args);

    void AnalyseParam(Proto::CreateComponent& proto) {}  // ������������ֹ�������ɱ����ģ��ĵݹ���ֹ��

private:
    std::vector<Thread*> _threads;      // �߳��б�
    size_t _threadIndex{ 0 };           // ʵ���̶߳������

    // ���������Ϣ���
    std::mutex _create_lock;            // �������ʱ�Ļ�����
    CacheSwap<Packet> _createPackets;   // ���ڻ�����Ϣ���Ľ���������
};

// �������
template<class T, typename ...TArgs>
inline void ThreadMgr::CreateComponent(TArgs ...args) {
    std::lock_guard<std::mutex> guard(_create_lock);    // �����ٽ���
    const std::string className = typeid(T).name();     // ��ȡ����

    // ���δע�ᣬ��ע������
    if (!ComponentFactory<TArgs...>::GetInstance()->IsRegisted(className)) {
        RegistToFactory<T, TArgs...>();                 
    }

    Proto::CreateComponent proto;                                                               // ����Proto����
    proto.set_class_name(className.c_str());                                                    // ��������
    AnalyseParam(proto, std::forward<TArgs>(args)...);                                          // ���������ò���
    auto pCreatePacket = MessageSystemHelp::CreatePacket(Proto::MsgId::MI_CreateComponent, 0);  // ������Ϣ��
    pCreatePacket->SerializeToBuffer(proto);                                                    // ���л���������
    _createPackets.GetWriterCache()->emplace_back(pCreatePacket);                               // ����Ϣ����ӵ�д������
}

// �������������ͣ�
template<typename ... Args>
void ThreadMgr::AnalyseParam(Proto::CreateComponent& proto, int value, Args... args) {
    auto pProtoParam = proto.mutable_params()->Add();           // ��Ӳ���
    pProtoParam->set_type(Proto::CreateComponentParam::Int);    // ���ò�������Ϊ����
    pProtoParam->set_int_param(value);                          // ���ò���ֵ
    AnalyseParam(proto, std::forward<Args>(args)...);           // �ݹ���ý�����һ������
}

// �����������ַ�����
template<typename ... Args>
void ThreadMgr::AnalyseParam(Proto::CreateComponent& proto, std::string value, Args... args) {
    auto pProtoParam = proto.mutable_params()->Add();               // ��Ӳ���
    pProtoParam->set_type(Proto::CreateComponentParam::String);     // ���ò�������Ϊ�ַ���
    pProtoParam->set_string_param(value.c_str());                   // ���ò���ֵ
    AnalyseParam(proto, std::forward<Args>(args)...);               // �ݹ���ý�����һ������
}
