#include "robot_console_login.h"
#include "robot.h"

#include "libserver/common.h"
#include "libserver/thread_mgr.h"

#include <iostream>
#include "global_robots.h"

// ע���������
void RobotConsoleLogin::RegisterHandler() {
    OnRegisterHandler("-a", BindFunP1(this, &RobotConsoleLogin::HandleLogin));				// ע�ᵥ���˺ŵ�¼���� "-a"
    OnRegisterHandler("-ex", BindFunP1(this, &RobotConsoleLogin::HandleLoginEx));			// ע�������˺ŵ�¼���� "-ex"
    OnRegisterHandler("-clean", BindFunP1(this, &RobotConsoleLogin::HandleLoginClean));		// ע��������е�¼�˺����� "-clean"
}

// ��ʾ������Ϣ
void RobotConsoleLogin::HandleHelp() {
    std::cout << "\t-a account.\t\tlogin by account" << std::endl;
    std::cout << "\t-ex account count.\tbatch login, account is prefix, count as number" << std::endl;
    std::cout << "\t-clean.\t\tclean all logined account." << std::endl;
}

// �������˺ŵ�¼
void RobotConsoleLogin::HandleLogin(std::vector<std::string>& params) {
    // �����������Ƿ���ȷ
    if (!CheckParamCnt(params, 1))
        return;

    ThreadMgr::GetInstance()->GetEntitySystem()->AddComponent<Robot>(params[0]);	// ������¼ʱ����¼�����̣߳���������cmd
    
    GlobalRobots::GetInstance()->SetRobotsCount(1);									// ����ȫ�ֻ���������Ϊ1
}

// ���������˺ŵ�¼
void RobotConsoleLogin::HandleLoginEx(std::vector<std::string>& params) const {
    // �����������Ƿ���ȷ
    if (!CheckParamCnt(params, 2))
        return;

    auto pThreadMgr = ThreadMgr::GetInstance();

    // ��ȡ������¼������
    const int count = std::atoi(params[1].c_str());
    if (count == 0) {
        std::cout << "input is error. see: -help" << std::endl;
        return;
    }

    // �������������˲���¼
    for (int i = 1; i <= count; i++) {
        const std::string account = params[0] + std::to_string(i);
        pThreadMgr->CreateComponent<Robot>(account);
    }

    // ����ȫ�ֻ���������Ϊ������¼������
    GlobalRobots::GetInstance()->SetRobotsCount(count);
}

// ������е�¼���˺�
void RobotConsoleLogin::HandleLoginClean(std::vector<std::string>& params) {
    std::cout << "close all." << std::endl;
    _threads.clear();									// ����߳�ID�б�
    GlobalRobots::GetInstance()->CleanRobotsCount();	// ���ȫ�ֻ���������
}
