#pragma once
#include "libserver/console.h"
#include <list>

// RobotConsoleLogin 类继承自 ConsoleCmd，处理机器人登录的控制台命令
class RobotConsoleLogin : public ConsoleCmd {
public:
    void RegisterHandler() override;	// 注册命令处理函数
    void HandleHelp() override;			// 显示帮助信息

private:
    void HandleLogin(std::vector<std::string>& params);				// 处理单个账号登录
    void HandleLoginEx(std::vector<std::string>& params) const;		// 处理批量账号登录
    void HandleLoginClean(std::vector<std::string>& params);		// 清除所有登录的账号

private:
    std::list<uint64> _threads;  // 用于存储线程ID的列表
};
