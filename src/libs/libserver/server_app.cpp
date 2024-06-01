#include "common.h"
#include "server_app.h"
#include "network_listen.h"

ServerApp::ServerApp(APP_TYPE apptype)
{
    // ȷ����������
    _appType = apptype;

    // ����ͨ���࣬�ṩͨ�õ������빦��
    Global::Instance();

    // �õ�ȫ��Ψһ���̹߳�����
    _pThreadMgr = ThreadMgr::Instance();

    // ����ʱ��
    UpdateTime();

    // �����߳�
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

// ���߳���ѭ��
void ServerApp::Run() const
{
    bool isRun = true;
    while(isRun)
    {
        UpdateTime(); //���µ�ǰ֡��ʱ��
        std::this_thread::sleep_for(std::chrono::milliseconds(10));
        isRun = _pThreadMgr->IsGameLoop();
    }
}


// ����ȫ��ʵ���е�ʱ��,Ҳ���Ǹ�����Ϸʱ��
void ServerApp::UpdateTime() const
{
    // ��ȡ��ǰʱ��㣬������ת��Ϊ�Ժ���Ϊ��λ��ʱ���
    auto timeValue = std::chrono::time_point_cast<std::chrono::milliseconds>(std::chrono::system_clock::now());
    
    // ��ȡȫ��ʵ���������� TimeTick 
    Global::GetInstance()->TimeTick = timeValue.time_since_epoch().count();

    // ��ʱ���ת��Ϊ time_t ����
    auto tt = std::chrono::system_clock::to_time_t(timeValue);

    // �� time_t ���͵�ʱ��ת��Ϊ struct tm �ṹ�壬��ʾ����ʱ��
    struct tm* ptm = localtime(&tt);

    // ����ȫ��ʵ���� YearDay ����ʾ��ǰ������һ���еĵڼ���
    Global::GetInstance()->YearDay = ptm->tm_yday;
}


//����һ��NetworkListen������, ������AddObjThread����������߳��� 
bool ServerApp::AddListenerToThread(std::string ip, int port) const
{
    NetworkListen* pListener = new NetworkListen();

    // �������ʧ�ܣ�ɾ�����NetworkListen������
    if(! pListener->Listen(ip, port))
    {
        delete pListener;
        return false;
    }

    // ��NetworkListen���������÷�������е�һ���߳���
    _pThreadMgr->AddObjToThread(pListener);
    return true;
}
