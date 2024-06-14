#pragma once

#include <thread>
#include <vector>
#include <queue>
#include <functional>

#include "system.h"
#include "disposable.h"

#include "entity.h"

#define ConsoleMaxBuffer 512

typedef std::function<void(std::vector<std::string>&)> HandleConsole;

class ConsoleCmd : public IDisposable
{
public:
	virtual void RegisterHandler() = 0;
	virtual void HandleHelp() = 0;

	void Dispose() override;
	void Process(std::vector<std::string>& params);

protected:
	void OnRegisterHandler(std::string key, HandleConsole handler);
	static bool CheckParamCnt(std::vector<std::string>& params, const size_t count);

private:
	std::map<std::string, HandleConsole> _handles;

};

class Console : public Entity<Console>, public IAwakeFromPoolSystem<>, public IUpdateSystem
{
public:
	void AwakeFromPool() override;
	void BackToPool() override;

	void Update() override;

	template<class T>
	void Register(std::string cmd);

protected:
	std::map<std::string, std::shared_ptr<ConsoleCmd>> _handles;

	std::mutex _lock;
	std::thread _thread;
	std::queue<std::string> _commands;

	bool _isRun{ true };
};

template<class T>
void Console::Register(std::string cmd)
{
	std::shared_ptr<T> pObj = std::make_shared<T>();	
	pObj->RegisterHandler();
	this->_handles[cmd] = pObj;
}

