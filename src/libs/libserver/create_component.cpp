#include "create_component.h"
#include "common.h"
#include "component_factory.h"
#include "packet.h"
#include "entity.h"
#include "message_component.h"
#include "system_manager.h"

// 组件工厂函数模板，使用索引序列展开参数包
template <typename... TArgs, size_t... Index>
IComponent* ComponentFactoryEx(EntitySystem* pEntitySystem, std::string className, const std::tuple<TArgs...>& args, std::index_sequence<Index...>)
{
    return pEntitySystem->AddComponentByName(className, std::get<Index>(args)...);
}

// 动态调用结构体模板，用于递归处理参数
template<size_t ICount>
struct DynamicCall
{
    // 动态调用函数，根据参数类型递归调用
    template <typename... TArgs>
    static IComponent* Invoke(EntitySystem* pEntitySystem, const std::string classname, std::tuple<TArgs...> t1, google::protobuf::RepeatedPtrField<::Proto::CreateComponentParam>& params)
    {
        // 如果参数为空，调用组件工厂函数
        if (params.size() == 0)
        {
            return ComponentFactoryEx(pEntitySystem, classname, t1, std::make_index_sequence<sizeof...(TArgs)>());
        }

        // 获取并移除第一个参数
        Proto::CreateComponentParam param = (*(params.begin()));
        params.erase(params.begin());

        // 根据参数类型递归调用
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

// 动态调用模板特化，当计数器为0时返回空指针
template<>
struct DynamicCall<0>
{
    // 特化的动态调用函数
    template <typename... TArgs>
    static IComponent* Invoke(EntitySystem* pEntitySystem, const std::string classname, std::tuple<TArgs...> t1, google::protobuf::RepeatedPtrField<::Proto::CreateComponentParam>& params)
    {
        return nullptr;
    }
};

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

// 从对象池中唤醒的方法实现
void CreateComponentC::AwakeFromPool()
{
    // 创建消息回调函数对象
    auto pMsgCallBack = new MessageCallBackFunction();
    // 添加消息组件
    AddComponent<MessageComponent>(pMsgCallBack);

    // 注册创建组件和移除组件的回调函数
    pMsgCallBack->RegisterFunction(Proto::MsgId::MI_CreateComponent, BindFunP1(this, &CreateComponentC::HandleCreateComponent));
    pMsgCallBack->RegisterFunction(Proto::MsgId::MI_RemoveComponent, BindFunP1(this, &CreateComponentC::HandleRemoveComponent));
}

// 返回对象池的方法实现
void CreateComponentC::BackToPool()
{

}

void CreateComponentC::HandleCreateComponent(Packet* pPacket) const
{
    // 解析消息包，获取创建组件的协议数据
    Proto::CreateComponent proto = pPacket->ParseToProto<Proto::CreateComponent>();
    const std::string className = proto.class_name();

    // 检查参数数量是否超过限制
    if (proto.params_size() >= 5)
    {
        std::cout << " !!!! CreateComponent failed. className:" << className.c_str() << " params size >= 5" << std::endl;
        return;
    }

    // 获取参数列表
    auto params = proto.params();
    
    // 动态调用组件工厂函数创建组件
    const auto pObj = DynamicCall<5>::Invoke(GetSystemManager()->GetEntitySystem(), className, std::make_tuple(), params);
    if (pObj == nullptr)
    {
        std::cout << " !!!! CreateComponent failed. className:" << className.c_str() << std::endl;
    }

    //std::cout << "CreateComponent. name:" << className << std::endl;
}

// 处理移除组件的函数实现
void CreateComponentC::HandleRemoveComponent(Packet* pPacket)
{
    // 解析消息包，获取移除组件的协议数据
    Proto::RemoveComponent proto = pPacket->ParseToProto<Proto::RemoveComponent>();
}
