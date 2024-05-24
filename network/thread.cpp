#include "thread.h"

bool Thread::Start()
{
    _isRun = true;
    _thread = std::thread(
        [this]()
        {
            while (_isRun)
            {
                Update();
            }
        });
    return true;
}

void Thread::Stop()
{
    if(_isRun)
        _isRun = false;
}

// 停止线程并等待线程执行完毕
void Thread::Dispose()
{
    Stop();
    if(_thread.joinable())  // 函数判断线程是否可被等待执行完毕
        _thread.join();  // 等待线程执行完毕
}

void Thread::Update()
{

}
