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

// ThreadMgr类继承自 Singleton 和 SystemManager
// 提供了启动所有线程、更新、检查停止状态、释放资源和创建线程的方法。
class ThreadMgr : public Singleton<ThreadMgr>, public SystemManager {
public:
    ThreadMgr();                                // 构造函数
    void StartAllThread();                      // 启动所有线程
    void Update() override;                     // 重写的更新方法
    bool IsStopAll();                           // 检查是否停止所有线程
    bool IsDisposeAll();                        // 检查是否释放所有资源
    void Dispose() override;                    // 重写的释放资源方法
    void CreateThread();                        // 创建线程

    // 创建组件
    template<class T, typename ...TArgs>
    void CreateComponent(TArgs... args);

    void DispatchPacket(Packet* pPacket);       // 分发消息包

private:
    // 解析参数（整型）
    template <typename...Args>
    void AnalyseParam(Proto::CreateComponent& proto, int value, Args...args);
    
    // 解析参数（字符串）
    template <typename...Args>
    void AnalyseParam(Proto::CreateComponent& proto, std::string value, Args...args);

    void AnalyseParam(Proto::CreateComponent& proto) {}  // 解析参数的终止条件（可变参数模板的递归终止）

private:
    std::vector<Thread*> _threads;      // 线程列表
    size_t _threadIndex{ 0 };           // 实现线程对象均分

    // 创建组件消息相关
    std::mutex _create_lock;            // 创建组件时的互斥锁
    CacheSwap<Packet> _createPackets;   // 用于缓存消息包的交换缓存区
};

// 创建组件
template<class T, typename ...TArgs>
inline void ThreadMgr::CreateComponent(TArgs ...args) {
    std::lock_guard<std::mutex> guard(_create_lock);    // 保护临界区
    const std::string className = typeid(T).name();     // 获取类名

    // 如果未注册，则注册该组件
    if (!ComponentFactory<TArgs...>::GetInstance()->IsRegisted(className)) {
        RegistToFactory<T, TArgs...>();                 
    }

    Proto::CreateComponent proto;                                                               // 创建Proto对象
    proto.set_class_name(className.c_str());                                                    // 设置类名
    AnalyseParam(proto, std::forward<TArgs>(args)...);                                          // 解析并设置参数
    auto pCreatePacket = MessageSystemHelp::CreatePacket(Proto::MsgId::MI_CreateComponent, 0);  // 创建消息包
    pCreatePacket->SerializeToBuffer(proto);                                                    // 序列化到缓冲区
    _createPackets.GetWriterCache()->emplace_back(pCreatePacket);                               // 将消息包添加到写缓存区
}

// 解析参数（整型）
template<typename ... Args>
void ThreadMgr::AnalyseParam(Proto::CreateComponent& proto, int value, Args... args) {
    auto pProtoParam = proto.mutable_params()->Add();           // 添加参数
    pProtoParam->set_type(Proto::CreateComponentParam::Int);    // 设置参数类型为整型
    pProtoParam->set_int_param(value);                          // 设置参数值
    AnalyseParam(proto, std::forward<Args>(args)...);           // 递归调用解析下一个参数
}

// 解析参数（字符串）
template<typename ... Args>
void ThreadMgr::AnalyseParam(Proto::CreateComponent& proto, std::string value, Args... args) {
    auto pProtoParam = proto.mutable_params()->Add();               // 添加参数
    pProtoParam->set_type(Proto::CreateComponentParam::String);     // 设置参数类型为字符串
    pProtoParam->set_string_param(value.c_str());                   // 设置参数值
    AnalyseParam(proto, std::forward<Args>(args)...);               // 递归调用解析下一个参数
}
