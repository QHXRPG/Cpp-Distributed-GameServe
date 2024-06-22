#pragma once
#include "libserver/common.h"
#include "libserver/component.h"
#include "libserver/system.h"
#include <curl/curl.h>
#include <json/json.h>  

enum HttpResquestState 
{
    HRS_Send,      // ��������
    HRS_Process,   // �ȴ�����
    HRS_Over,      // ���
    HRS_NoActive,	  // ��ɺ�ķǼ���״̬���ȴ��߳�ɾ��
    HRS_Timeout,	  // ����ʱ
};

enum HttpResquestMethod
{
    HRM_Post,
    HRM_Get,
};


enum CURLMRS
{
    CRS_None,
    CRS_OK,
    CRS_CURLM_CALL_MULTI_PERFORM, // call curl_multi_perform
    CRS_TIMEOUT,
    CRS_CURLMError, // curlm ���ó���
    CRS_SelectError,
};

class HttpRequest :public Component<HttpRequest>, public IUpdateSystem
{
public:
    void Update() override;
    void BackToPool() override;

protected:
    bool ProcessSend();
    void ProcessTimeout() const;
    virtual bool ProcessOver();

    virtual bool Process();
    virtual void ProcessMsg();
    virtual void ProcessMsg(Json::Value value) = 0;

protected:
    HttpResquestMethod _method{ HRM_Get };
    CURL* _pCurl{ nullptr };
    CURLM *_pMultiHandle{ nullptr };
    CURLMRS _curlRs;

    std::string _url, _params;
    std::string _responseBuffer;
    HttpResquestState State{ HRS_Send };

    std::string _account;
};

/**
* ʹ��select��������multi curl�ļ���������״̬
* �����ɹ�����0������ʧ�ܷ���-1
*/
inline CURLMRS curl_multi_select(CURLM * curl_m)
{
    struct timeval timeout_tv;
    fd_set  fd_read;
    fd_set  fd_write;
    fd_set  fd_except;
    int     max_fd = -1;

    // ע������һ��Ҫ���fdset,curl_multi_fdset����ִ��fdset����ղ���  //
    FD_ZERO(&fd_read);
    FD_ZERO(&fd_write);
    FD_ZERO(&fd_except);

    // ����select��ʱʱ��
    timeout_tv.tv_sec = 1;
    timeout_tv.tv_usec = 0;

    // ��ȡmulti curl��Ҫ�������ļ����������� fd_set //
    curl_multi_fdset(curl_m, &fd_read, &fd_write, &fd_except, &max_fd);

    /**
    * When max_fd returns with -1,
    * you need to wait a while and then proceed and call curl_multi_perform anyway.
    * How long to wait? I would suggest 100 milliseconds at least,
    * but you may want to test it out in your own particular conditions to find a suitable value.
    */
    if (-1 == max_fd)
    {
        return CURLMRS::CRS_CURLM_CALL_MULTI_PERFORM;
    }

    /**
    * ִ�м��������ļ�������״̬�����ı��ʱ�򷵻�
    * ����0���������curl_multi_perform֪ͨcurlִ����Ӧ����
    * ����-1����ʾselect����
    * ע�⣺��ʹselect��ʱҲ��Ҫ����0���������ȥ�������ĵ�˵��
    */
    int ret_code = ::select(max_fd + 1, &fd_read, &fd_write, &fd_except, &timeout_tv);
    switch (ret_code)
    {
    case -1:
        /* select error */
        return CRS_SelectError;
    case 0:
        /* select timeout */
        return CRS_TIMEOUT;
    default:
        /* one or more of curl's file descriptors say there's data to read or write*/
        return CRS_OK;
    }
}