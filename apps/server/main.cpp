/* 
说明： 
功能：程序入口
作者：QHXRPG
日期：2024/5/24
*/

#include "server.h"
#include <signal.h>


bool isRun = true;

void stop(int sig)
{
    isRun = false;
}

int main()
{
    // 设置退出信号量
    signal(SIGINT, stop);
    Server server;

    // 启动监听服务
    if(! server.Listen("127.0.0.0", 2233))
        return 1;

    while(isRun)
    {
        if(! server.Update())
            break;
        server.DataHandler();
    }

    server.Dispose();
    return 0;
}