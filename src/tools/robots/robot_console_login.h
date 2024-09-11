#pragma once
#include "libserver/console.h"
#include <list>

// RobotConsoleLogin ��̳��� ConsoleCmd����������˵�¼�Ŀ���̨����
class RobotConsoleLogin : public ConsoleCmd {
public:
    void RegisterHandler() override;	// ע���������
    void HandleHelp() override;			// ��ʾ������Ϣ

private:
    void HandleLogin(std::vector<std::string>& params);				// �������˺ŵ�¼
    void HandleLoginEx(std::vector<std::string>& params) const;		// ���������˺ŵ�¼
    void HandleLoginClean(std::vector<std::string>& params);		// ������е�¼���˺�

private:
    std::list<uint64> _threads;  // ���ڴ洢�߳�ID���б�
};
