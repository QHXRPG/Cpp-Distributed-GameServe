#pragma once
#include "disposable.h"
#include "thread_mgr.h"
#include "common.h"


// 封装所有服务的 例行功能
template<class APPClass>
inline int MainTemplate()
{
    // 创建一个名为 APPClass 的服务
    APPClass* pApp = new APPClass();

    // 初始化服务（创建线程池等）
    pApp->InitApp();

    // 启动该服务所有的线程
    pApp->StartAllThread();
    
    // 运行 Run() 服务循环， 程序会一直在这里循环，直到收到退出信号 
    pApp->Run();

    // 关闭并销毁服务
    pApp->Dispose();
    delete pApp;
    return 0;
}

// 服务基类，基层该类的都是各种服务：登录服务、地图服务、game服务等
class ServerApp : public IDisposable
{
protected:
    ThreadMgr* _pThreadMgr;
    APP_TYPE _appType;

public:
    ServerApp(APP_TYPE apptype);
    ~ServerApp();

    // 服务子类必须初始化
    virtual void InitApp() = 0;
    void Dispose() override;

    void StartAllThread() const;
    void Run() const;
    void UpdateTime() const;

    bool AddListenerToThread(std::string ip, int port) const;
};




