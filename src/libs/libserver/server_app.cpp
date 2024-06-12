#include "common.h"
#include "server_app.h"
#include "network_listen.h"
#include "object_pool_mgr.h"

ServerApp::ServerApp(APP_TYPE  appType)
{
    signal(SIGINT, Signalhandler);

    _appType = appType;

    DynamicObjectPoolMgr::Instance();
    Global::Instance();
    ThreadMgr::Instance();
    _pThreadMgr = ThreadMgr::GetInstance();
    UpdateTime();

    // �����߳�
    for (int i = 0; i < 3; i++)
    {
        _pThreadMgr->NewThread();
    }

    _pThreadMgr->StartAllThread();
}

ServerApp::~ServerApp()
{
    _pThreadMgr->DestroyInstance();
}

void ServerApp::Signalhandler(const int signalValue)
{
    switch (signalValue)
    {
#if ENGINE_PLATFORM != PLATFORM_WIN32
    case SIGSTOP:
    case SIGQUIT:
#endif

    case SIGTERM:
    case SIGINT:
        Global::GetInstance()->IsStop = true;
        break;
    }

    std::cout << "\nrecv signal. value:" << signalValue << " Global IsStop::" << Global::GetInstance()->IsStop << std::endl;
}

void ServerApp::Dispose()
{
    _pThreadMgr->Dispose();
}

void ServerApp::Run() const
{
    while (!Global::GetInstance()->IsStop)
    {
        UpdateTime();
        _pThreadMgr->Update();
        DynamicObjectPoolMgr::GetInstance()->Update();
        std::this_thread::sleep_for(std::chrono::milliseconds(1));
    }

    // ֹͣ�����߳�
    std::cout << "stoping all threads..." << std::endl;
    bool isStop;
    do
    {
        isStop = _pThreadMgr->IsStopAll();
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
    } while (!isStop);

    // �ͷ������߳���Դ
    std::cout << "disposing all threads..." << std::endl;

    // 1.���߳���Դ
    bool isDispose;
    do
    {
        isDispose = _pThreadMgr->IsDisposeAll();
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
    } while (!isDispose);

    // 2.���߳���Դ
    _pThreadMgr->Dispose();

    std::cout << "disposing all pool..." << std::endl;
    DynamicObjectPoolMgr::GetInstance()->Update();
    DynamicObjectPoolMgr::GetInstance()->Dispose();
    DynamicObjectPoolMgr::DestroyInstance();

    Global::DestroyInstance();
    ThreadMgr::DestroyInstance();
}


void ServerApp::UpdateTime() const
{
#if ENGINE_PLATFORM != PLATFORM_WIN32
    struct timeval tv;
    gettimeofday(&tv, nullptr);
    Global::GetInstance()->TimeTick = tv.tv_sec * 1000 +  tv.tv_usec * 0.001;
#else
    auto timeValue = std::chrono::time_point_cast<std::chrono::milliseconds>(std::chrono::system_clock::now());
    Global::GetInstance()->TimeTick = timeValue.time_since_epoch().count();
#endif

//#if ENGINE_PLATFORM != PLATFORM_WIN32
//    auto tt = std::chrono::system_clock::to_time_t(timeValue);
//    struct tm* ptm = localtime(&tt);
//    Global::GetInstance()->YearDay = ptm->tm_yday;
//#else
//    auto tt = std::chrono::system_clock::to_time_t(timeValue);
//    struct tm tm;
//    localtime_s(&tm, &tt);
//    Global::GetInstance()->YearDay = tm.tm_yday;
//#endif
}

bool ServerApp::AddListenerToThread(std::string ip, int port) const
{
    NetworkListen* pListener = new NetworkListen();
    if (!pListener->Listen(ip, port))
    {
        delete pListener;
        return false;
    }

    _pThreadMgr->AddNetworkToThread(APP_Listen, pListener);
    return true;
}
