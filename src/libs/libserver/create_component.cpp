#include "create_component.h"
#include "common.h"
#include "component_factory.h"
#include "packet.h"
#include "entity.h"
#include "message_component.h"
#include "system_manager.h"

// �����������ģ�壬ʹ����������չ��������
template <typename... TArgs, size_t... Index>
IComponent* ComponentFactoryEx(EntitySystem* pEntitySystem, std::string className, const std::tuple<TArgs...>& args, std::index_sequence<Index...>)
{
    return pEntitySystem->AddComponentByName(className, std::get<Index>(args)...);
}

// ��̬���ýṹ��ģ�壬���ڵݹ鴦�����
template<size_t ICount>
struct DynamicCall
{
    // ��̬���ú��������ݲ������͵ݹ����
    template <typename... TArgs>
    static IComponent* Invoke(EntitySystem* pEntitySystem, const std::string classname, std::tuple<TArgs...> t1, google::protobuf::RepeatedPtrField<::Proto::CreateComponentParam>& params)
    {
        // �������Ϊ�գ����������������
        if (params.size() == 0)
        {
            return ComponentFactoryEx(pEntitySystem, classname, t1, std::make_index_sequence<sizeof...(TArgs)>());
        }

        // ��ȡ���Ƴ���һ������
        Proto::CreateComponentParam param = (*(params.begin()));
        params.erase(params.begin());

        // ���ݲ������͵ݹ����
        if (param.type() == Proto::CreateComponentParam::Int)
        {
            auto t2 = std::tuple_cat(t1, std::make_tuple(param.int_param()));
            return DynamicCall<ICount - 1>::Invoke(pEntitySystem, classname, t2, params);
        }

        if (param.type() == Proto::CreateComponentParam::String)
        {
            auto t2 = std::tuple_cat(t1, std::make_tuple(param.string_param()));
            return DynamicCall<ICount - 1>::Invoke(pEntitySystem, classname, t2, params);
        }

        return nullptr;
    }
};

// ��̬����ģ���ػ�����������Ϊ0ʱ���ؿ�ָ��
template<>
struct DynamicCall<0>
{
    // �ػ��Ķ�̬���ú���
    template <typename... TArgs>
    static IComponent* Invoke(EntitySystem* pEntitySystem, const std::string classname, std::tuple<TArgs...> t1, google::protobuf::RepeatedPtrField<::Proto::CreateComponentParam>& params)
    {
        return nullptr;
    }
};

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

// �Ӷ�����л��ѵķ���ʵ��
void CreateComponentC::AwakeFromPool()
{
    // ������Ϣ�ص���������
    auto pMsgCallBack = new MessageCallBackFunction();
    // �����Ϣ���
    AddComponent<MessageComponent>(pMsgCallBack);

    // ע�ᴴ��������Ƴ�����Ļص�����
    pMsgCallBack->RegisterFunction(Proto::MsgId::MI_CreateComponent, BindFunP1(this, &CreateComponentC::HandleCreateComponent));
    pMsgCallBack->RegisterFunction(Proto::MsgId::MI_RemoveComponent, BindFunP1(this, &CreateComponentC::HandleRemoveComponent));
}

// ���ض���صķ���ʵ��
void CreateComponentC::BackToPool()
{

}

void CreateComponentC::HandleCreateComponent(Packet* pPacket) const
{
    // ������Ϣ������ȡ���������Э������
    Proto::CreateComponent proto = pPacket->ParseToProto<Proto::CreateComponent>();
    const std::string className = proto.class_name();

    // �����������Ƿ񳬹�����
    if (proto.params_size() >= 5)
    {
        std::cout << " !!!! CreateComponent failed. className:" << className.c_str() << " params size >= 5" << std::endl;
        return;
    }

    // ��ȡ�����б�
    auto params = proto.params();
    
    // ��̬����������������������
    const auto pObj = DynamicCall<5>::Invoke(GetSystemManager()->GetEntitySystem(), className, std::make_tuple(), params);
    if (pObj == nullptr)
    {
        std::cout << " !!!! CreateComponent failed. className:" << className.c_str() << std::endl;
    }

    //std::cout << "CreateComponent. name:" << className << std::endl;
}

// �����Ƴ�����ĺ���ʵ��
void CreateComponentC::HandleRemoveComponent(Packet* pPacket)
{
    // ������Ϣ������ȡ�Ƴ������Э������
    Proto::RemoveComponent proto = pPacket->ParseToProto<Proto::RemoveComponent>();
}
