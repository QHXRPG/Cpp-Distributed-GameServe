/* 
说明： 
功能：机器人测试
作者：QHXRPG
日期：2024/5/24
*/

#include <atomic>
#include <iostream>
#include <thread>
#include <string>
#include <vector>
#include "client_thread.h"
#include "network/network.h"
#include "network/thread.h"

int main(int argc, char *argv[])
{
	int threadNum = 1, msgCount = 1;

    // 设置线程数量
	if (argc >= 2)
	{
		threadNum = std::atoi(argv[1]);
	}

	if (argc == 3)
	{
		msgCount = std::atoi(argv[2]);
	}
    
    // 创建线程池，线程数量为：threadNum
	std::vector<ClientThread*> threads(threadNum);

	for (auto i = 0; i < threadNum; ++i)
	{
		threads[i] = new ClientThread(msgCount);
		threads[i]->Start();
	}

	std::cout << "在线的socket数量:" << threads.size() << "\t已完成的:" 
		<< (threadNum - threads.size()) * 100.0f / threadNum << "%" << std::endl;
	while (true)
	{
        // 线程池中的线程都被销毁了，退出循环
		if (threads.size() <= 0)
		{
			break;
		}

		auto iter = threads.begin();
		while (iter != threads.end())
		{
			ClientThread* pThread = (*iter);

            // 如果线程停止了，则销魂该线程
			if (!pThread->IsRun())
			{
				pThread->Dispose();
				delete pThread;
				iter = threads.erase(iter);

				std::cout << "在线的socket数量:" << threads.size() 
                        << "\t已完成的:" 
                        << (threadNum - threads.size()) * 100.0f / threadNum 
                        << "%" << std::endl;

				continue;
			}

			++iter;
		}
		std::this_thread::sleep_for(std::chrono::milliseconds(100));
	}

	threads.clear();
	return 0;
}