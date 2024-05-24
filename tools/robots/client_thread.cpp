#include <iostream>
#include "client.h"
#include "client_thread.h"

ClientThread::ClientThread(int msgCount)
{
    _msgCount = msgCount;
}

void ClientThread::Update()
{
    // 如果客户端不存在，重新创建并连接到服务端
    if(_Client == nullptr)
    {
        _Client = new Client(_msgCount, _thread.get_id());
        if(! _Client->Connect("127.0.0.1", 2233))
        {
            std::cout << " 连接失败" << std::endl;
        }
    }
    else
    {
        _Client->Update();
        _Client->DataHandler();
        if(_Client->IsCompleted())
        {
            Thread::Stop();
        }
    }
    std::this_thread::sleep_for(std::chrono::milliseconds(10));
}

void ClientThread::Dispose()
{
    if (_Client != nullptr)
	{
		_Client->Dispose();
		delete _Client;
		_Client = nullptr;
	}

	Thread::Dispose();
}
