#include "http_request.h"
#include "libserver/thread_mgr.h"
#include "libserver/packet.h"
#include "libserver/message_system.h"

size_t WriteFunction(void* buffer, size_t size, size_t nmemb, void* lpVoid)
{
    std::string* str = static_cast<std::string*>(lpVoid);
    if (nullptr == str || nullptr == buffer)
    {
        return -1;
    }

    char* pData = static_cast<char*>(buffer);
    str->append(pData, size * nmemb);
    return size * nmemb;
}

void HttpRequest::BackToPool()
{
    _responseBuffer = "";
    State = HttpResquestState::HRS_Send;

    if (_pMultiHandle != nullptr && _pCurl != nullptr )
        curl_multi_remove_handle(_pMultiHandle, _pCurl);

    if (_pCurl != nullptr)
        curl_easy_cleanup(_pCurl);

    if (_pMultiHandle != nullptr)
        curl_multi_cleanup(_pMultiHandle);

    _pCurl = nullptr;
    _pMultiHandle = nullptr;
}

void HttpRequest::Update()
{
    switch (State)
    {
    case HRS_Send:
    {
        if (ProcessSend())
            State = HRS_Process;
    }
    break;
    case HRS_Process:
    {
        if (Process())
            State = HRS_Over;
    }
    break;
    case HRS_Over:
    {
        ProcessOver();
        State = HRS_NoActive;
        _active = false;
    }
    break;
    case HRS_Timeout:
    {
        ProcessTimeout();
        State = HRS_NoActive;
        _active = false;
    }
    break;
    default:
        break;
    }
}

bool HttpRequest::ProcessSend()
{
    _pCurl = curl_easy_init();
    _pMultiHandle = curl_multi_init();

    curl_easy_setopt(_pCurl, CURLOPT_URL, _url.c_str());
    curl_easy_setopt(_pCurl, CURLOPT_READFUNCTION, NULL);
    curl_easy_setopt(_pCurl, CURLOPT_WRITEFUNCTION, WriteFunction);  // ����server�ķ��ص����ݵĽ��շ�ʽ
    curl_easy_setopt(_pCurl, CURLOPT_WRITEDATA, static_cast<void*>(&_responseBuffer));

    curl_easy_setopt(_pCurl, CURLOPT_NOSIGNAL, 1);
    curl_easy_setopt(_pCurl, CURLOPT_VERBOSE, 0); //��ӡ������Ϣ  
    curl_easy_setopt(_pCurl, CURLOPT_HEADER, 0); //����Ӧͷ��Ϣ����Ӧ��һ�𴫸�write_data  
                                                 //curl_easy_setopt( _pCurl, CURLOPT_CONNECTTIMEOUT, 3 );
                                                 //curl_easy_setopt( _pCurl, CURLOPT_TIMEOUT, 3 );

    if (_method == HttpResquestMethod::HRM_Post)
    {
        curl_easy_setopt(_pCurl, CURLOPT_POST, true); //�����ʷ�0��ʾ���β���Ϊpost 
        curl_easy_setopt(_pCurl, CURLOPT_POSTFIELDS, _params.c_str()); //post����
    }

    curl_multi_add_handle(_pMultiHandle, _pCurl);

    State = HttpResquestState::HRS_Process;
    return true;

}

void HttpRequest::ProcessTimeout() const
{
    Proto::AccountCheckToHttpRs checkProto;
    checkProto.set_account(_account);
    checkProto.set_return_code(Proto::AccountCheckReturnCode::ARC_TIMEOUT);

    MessageSystemHelp::DispatchPacket(Proto::MsgId::MI_AccountCheckToHttpRs, 0, checkProto);
}

bool HttpRequest::ProcessOver()
{
    curl_multi_remove_handle(_pMultiHandle, _pCurl);
    curl_easy_cleanup(_pCurl);
    curl_multi_cleanup(_pMultiHandle);

    _pCurl = nullptr;
    _pMultiHandle = nullptr;

    return true;
}

bool HttpRequest::Process()
{
    int running_handle_count;
    CURLMcode curlMcode = curl_multi_perform(_pMultiHandle, &running_handle_count);
    if (CURLM_CALL_MULTI_PERFORM == curlMcode)
        return false;

    // ��������
    if (curlMcode != CURLMcode::CURLM_OK)
    {
        _curlRs = CRS_CURLMError;
        State = HRS_Timeout;
        return true;
    }

    // �������ˣ����ٴ���
    if (running_handle_count == 0)
    {
        ProcessMsg();
        return true;
    }

    CURLMRS rs = curl_multi_select(_pMultiHandle);
    if (rs != CRS_OK && rs != CRS_CURLM_CALL_MULTI_PERFORM)
    {
        _curlRs = rs;
        State = HRS_Timeout;
        return false;
    }

    // ��һFrame����ִ��
    return false;
}

void HttpRequest::ProcessMsg()
{
    int         msgs_left;
    CURLMsg* msg;
    msg = curl_multi_info_read(_pMultiHandle, &msgs_left);
    if (CURLMSG_DONE == msg->msg)
    {
        //if ( msg->easy_handle != _pCurl )
        //    std::cerr << "curl not found" << std::endl;

        //INFO_MSG( fmt::format( "curl completed with status:{}\n", msg->data.result ) );
        //std::cout << "curl context:" << _responseBuffer.c_str() << std::endl;

        Json::CharReaderBuilder readerBuilder;
        Json::CharReader* jsonReader = readerBuilder.newCharReader();

        Json::Value value;
        JSONCPP_STRING errs;

        bool ok = jsonReader->parse(_responseBuffer.data(), _responseBuffer.data() + _responseBuffer.size(), &value, &errs);
        if (ok && errs.size() == 0)
        {
            ProcessMsg(value);
        }
        else
        {
            std::cout << "json parse failed. " << _responseBuffer.c_str() << std::endl;
        }

        delete jsonReader;
    }
}

