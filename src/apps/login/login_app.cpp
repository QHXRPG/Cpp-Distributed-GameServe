#include "login_app.h"
#include <iostream>
#include "test_msg_handler.h"

void LoginApp::InitApp()
{
    AddListenerToThread("127.0.0.1", 2233);

    // 此处的 _pThreadMgr是在 LoginApp 的构造函数当中创建，由LoginApp类管理
    _pThreadMgr->AddObjToThread(new TestMsgHandler());
}
