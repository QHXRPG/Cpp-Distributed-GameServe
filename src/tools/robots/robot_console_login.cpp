#include "robot_console_login.h"
#include "robot.h"

#include "libserver/common.h"
#include "libserver/thread_mgr.h"

#include <iostream>
#include "global_robots.h"

// 注册命令处理函数
void RobotConsoleLogin::RegisterHandler() {
    OnRegisterHandler("-a", BindFunP1(this, &RobotConsoleLogin::HandleLogin));				// 注册单个账号登录命令 "-a"
    OnRegisterHandler("-ex", BindFunP1(this, &RobotConsoleLogin::HandleLoginEx));			// 注册批量账号登录命令 "-ex"
    OnRegisterHandler("-clean", BindFunP1(this, &RobotConsoleLogin::HandleLoginClean));		// 注册清除所有登录账号命令 "-clean"
}

// 显示帮助信息
void RobotConsoleLogin::HandleHelp() {
    std::cout << "\t-a account.\t\tlogin by account" << std::endl;
    std::cout << "\t-ex account count.\tbatch login, account is prefix, count as number" << std::endl;
    std::cout << "\t-clean.\t\tclean all logined account." << std::endl;
}

// 处理单个账号登录
void RobotConsoleLogin::HandleLogin(std::vector<std::string>& params) {
    // 检查参数数量是否正确
    if (!CheckParamCnt(params, 1))
        return;

    ThreadMgr::GetInstance()->GetEntitySystem()->AddComponent<Robot>(params[0]);	// 单独登录时，登录到主线程，方便输入cmd
    
    GlobalRobots::GetInstance()->SetRobotsCount(1);									// 设置全局机器人数量为1
}

// 处理批量账号登录
void RobotConsoleLogin::HandleLoginEx(std::vector<std::string>& params) const {
    // 检查参数数量是否正确
    if (!CheckParamCnt(params, 2))
        return;

    auto pThreadMgr = ThreadMgr::GetInstance();

    // 获取批量登录的数量
    const int count = std::atoi(params[1].c_str());
    if (count == 0) {
        std::cout << "input is error. see: -help" << std::endl;
        return;
    }

    // 批量创建机器人并登录
    for (int i = 1; i <= count; i++) {
        const std::string account = params[0] + std::to_string(i);
        pThreadMgr->CreateComponent<Robot>(account);
    }

    // 设置全局机器人数量为批量登录的数量
    GlobalRobots::GetInstance()->SetRobotsCount(count);
}

// 清除所有登录的账号
void RobotConsoleLogin::HandleLoginClean(std::vector<std::string>& params) {
    std::cout << "close all." << std::endl;
    _threads.clear();									// 清空线程ID列表
    GlobalRobots::GetInstance()->CleanRobotsCount();	// 清除全局机器人数量
}
