#include "common.h"
#include "server_app.h"
#include "network_listen.h"

ServerApp::ServerApp(APP_TYPE apptype)
{
    // 确定服务类型
    _appType = apptype;

    // 数据通用类，提供通用的数据与功能
    Global::Instance();

    // 拿到全局唯一的线程管理器
    _pThreadMgr = ThreadMgr::Instance();

    // 更新时间
    UpdateTime();

    // 创建线程
    for(int i=0; i<3; i++)
    {
        _pThreadMgr->NewThread();
    }
}

ServerApp::~ServerApp()
{
}

void ServerApp::Dispose()
{
}

void ServerApp::StartAllThread() const
{
}

// 多线程主循环
void ServerApp::Run() const
{
    bool isRun = true;
    while(isRun)
    {
        UpdateTime(); //更新当前帧的时间
        std::this_thread::sleep_for(std::chrono::milliseconds(10));
        isRun = _pThreadMgr->IsGameLoop();
    }
}


// 更新全局实例中的时间,也就是更新游戏时间
void ServerApp::UpdateTime() const
{
    // 获取当前时间点，并将其转换为以毫秒为单位的时间点
    auto timeValue = std::chrono::time_point_cast<std::chrono::milliseconds>(std::chrono::system_clock::now());
    
    // 获取全局实例并更新其 TimeTick 
    Global::GetInstance()->TimeTick = timeValue.time_since_epoch().count();

    // 将时间点转换为 time_t 类型
    auto tt = std::chrono::system_clock::to_time_t(timeValue);

    // 将 time_t 类型的时间转换为 struct tm 结构体，表示本地时间
    struct tm* ptm = localtime(&tt);

    // 更新全局实例的 YearDay ，表示当前日期是一年中的第几天
    Global::GetInstance()->YearDay = ptm->tm_yday;
}


//创建一个NetworkListen监听类, 并调用AddObjThread将该类放入线程中 
bool ServerApp::AddListenerToThread(std::string ip, int port) const
{
    NetworkListen* pListener = new NetworkListen();

    // 如果监听失败，删除这个NetworkListen监听类
    if(! pListener->Listen(ip, port))
    {
        delete pListener;
        return false;
    }

    // 将NetworkListen监听类放入该服务的其中的一个线程中
    _pThreadMgr->AddObjToThread(pListener);
    return true;
}
