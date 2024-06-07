#pragma once
#include <vector>
#include <algorithm>
#include <list>
#include "disposable.h"


// ���ڶ��������\ɾ��
template<class T>
class CacheRefresh :public IDisposable
{
public:
	std::vector<T*>* GetAddCache();
	std::vector<T*>* GetRemoveCache();
	std::vector<T*>* GetReaderCache();

	// ����ɾ����Obj�������Ƿ����ڴ���մ���
	std::list<T*> Swap();
	bool CanSwap();
    void Dispose() override;

protected:
	std::vector<T*> _reader;
	std::vector<T*> _add;
	std::vector<T*> _remove;
};

template <class T>
inline std::vector<T*>* CacheRefresh<T>::GetAddCache()
{
	return &_add;
}

template <class T>
inline std::vector<T*>* CacheRefresh<T>::GetRemoveCache()
{
	return &_remove;
}

template <class T>
inline std::vector<T*>* CacheRefresh<T>::GetReaderCache()
{
	return &_reader;
}

// ��ɾ�������ӵ�����Ӧ�õ��������С�
// ������Ҫɾ���Ķ������߳���ͳһɾ�ͷ�
template <class T>
inline std::list<T*> CacheRefresh<T>::Swap()
{
	std::list<T*> rs;

	// ����_add���Ӷ�����Ԫ��
	for (auto one : _add)
	{
		_reader.push_back(one);
	}
	_add.clear();

	// ����_removeɾ��������Ԫ��
	for (auto one : _remove)
	{
		//���� _reader ���������ҵ�һ������ one ��Ԫ�أ�������һ��ָ���Ԫ�صĵ�����
		auto iterReader = std::find_if(_reader.begin(), _reader.end(), [one](auto x)
		{
			return x == one;
		});

		if (iterReader == _reader.end())
		{
            std::cout << "CacheRefresh Swap error. not find obj to remove. sn:" 
						<< one->GetSN() << std::endl;
		}
		else
		{
			rs.push_back(one);
			_reader.erase(iterReader);
		}
	}
	_remove.clear();
	return rs;
}

template <class T>
inline bool CacheRefresh<T>::CanSwap()
{
	return _add.size() > 0 || _remove.size() > 0;
}

template<class T>
inline void CacheRefresh<T>::Dispose()
{
    for (auto iter = _add.begin(); iter != _add.end(); ++iter)
    {
        (*iter)->Dispose();
        delete (*iter);
    }
    _add.clear();

    for (auto iter = _remove.begin(); iter != _remove.end(); ++iter)
    {
        (*iter)->Dispose();
        delete (*iter);
    }
    _remove.clear();

    for (auto iter = _reader.begin(); iter != _reader.end(); ++iter)
    {
        (*iter)->Dispose();
        delete (*iter);
    }
    _reader.clear();
}
