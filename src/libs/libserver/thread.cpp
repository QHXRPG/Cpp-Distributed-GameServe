#include "thread.h"
#include "global.h"

#include <iterator>

void Thread::Start( ) 
{
	_isRun = true;
	_thread = std::thread([this](){
		while(_isRun)
		{
			Update();
		}
	});
}

void Thread::Stop( ) {
	if ( _isRun )
	{
		_isRun = false;
		if(_thread.joinable())
			_thread.join();
	}
}

Thread::Thread()
{
	this->_isRun = true;
	_tmpObjs.clear();
}

bool Thread::IsRun() const
{
	return _isRun;
}

void Thread::AddThreadObj(ThreadObject* obj)
{
	obj->RegisterMsgFunction();
	std::lock_guard<std::mutex> guard(_thread_lock);
	_objlist.push_back(obj);
}

void Thread::Dispose( ) 
{
	// ɾ��_objlist�еĶ���
    std::list<ThreadObject*>::iterator iter = _objlist.begin();
    while (iter != _objlist.end()) {
        (*iter)->Dispose();
        delete (*iter);
        iter = _objlist.erase(iter);
    }

    _objlist.clear();
}

// ���¸��̵߳��еĶ���
void Thread::Update( ) 
{
	_thread_lock.lock();
	std::copy(_objlist.begin(), _objlist.end(), std::back_inserter(_tmpObjs));
	_thread_lock.unlock();

	for(ThreadObject *tmpObj : _tmpObjs)
	{
		tmpObj->Update();

		// �Ǽ���״̬��ɾ��
		if(!tmpObj->IsActive())
		{
			_thread_lock.lock();
			_objlist.remove(tmpObj);
			_thread_lock.unlock();

			tmpObj->Dispose();
			delete tmpObj;
		}
	}
	_tmpObjs.clear();
	std::this_thread::sleep_for(std::chrono::milliseconds(1));
}
