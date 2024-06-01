#pragma once
#include "disposable.h"
#include "thread_mgr.h"
#include "common.h"


// ��װ���з���� ���й���
template<class APPClass>
inline int MainTemplate()
{
    // ����һ����Ϊ APPClass �ķ���
    APPClass* pApp = new APPClass();

    // ��ʼ�����񣨴����̳߳صȣ�
    pApp->InitApp();

    // �����÷������е��߳�
    pApp->StartAllThread();
    
    // ���� Run() ����ѭ���� �����һֱ������ѭ����ֱ���յ��˳��ź� 
    pApp->Run();

    // �رղ����ٷ���
    pApp->Dispose();
    delete pApp;
    return 0;
}

// ������࣬�������Ķ��Ǹ��ַ��񣺵�¼���񡢵�ͼ����game�����
class ServerApp : public IDisposable
{
protected:
    ThreadMgr* _pThreadMgr;
    APP_TYPE _appType;

public:
    ServerApp(APP_TYPE apptype);
    ~ServerApp();

    // ������������ʼ��
    virtual void InitApp() = 0;
    void Dispose() override;

    void StartAllThread() const;
    void Run() const;
    void UpdateTime() const;

    bool AddListenerToThread(std::string ip, int port) const;
};




