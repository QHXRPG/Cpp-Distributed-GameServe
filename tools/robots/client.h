/* 
说明： 
功能：客户端功能类
作者：QHXRPG
日期：2024/5/24
*/

#pragma once

#include <list>
#include "network/network_connector.h"
#include <random>
#include <thread>

class Client : public NetworkConnector
{
public:
    // explicit 声明构造函数为显式构造函数(必须采用该构造方法)
    explicit Client(int msgCount, std::thread::id threadId);
    void DataHandler();
    bool IsCompleted() const
    {
        return _isCompleted;
    }

private:
    std::string GenRandom();
    int _msgCount;
    int _index{0};
    bool _isCompleted{false};
    std::string _lastMsg{""};
    std::default_random_engine* _RandomEngine;  //生成伪随机数
};