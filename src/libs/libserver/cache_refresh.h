#pragma once
#include <vector>
#include <algorithm>
#include <list>
#include <iostream>

// CacheRefresh模板类，用于管理对象的缓存
template<class T>
class CacheRefresh {
public:
    std::vector<T*>* GetAddCache();		// 获取添加缓存
    std::vector<T*>* GetRemoveCache();	// 获取移除缓存
    std::vector<T*>* GetReaderCache();	// 获取读取缓存
    std::list<T*> Swap();				// 交换缓存，返回被移除的对象列表
    bool CanSwap();						// 判断是否可以交换缓存
    void BackToPool();					// 将对象返回到池中

protected:
    std::vector<T*> _reader;  // 读取缓存
    std::vector<T*> _add;     // 添加缓存
    std::vector<T*> _remove;  // 移除缓存
};

// 获取添加缓存
template <class T>
inline std::vector<T*>* CacheRefresh<T>::GetAddCache() {
    return &_add;
}

// 获取移除缓存
template <class T>
inline std::vector<T*>* CacheRefresh<T>::GetRemoveCache() {
    return &_remove;
}

// 获取读取缓存
template <class T>
inline std::vector<T*>* CacheRefresh<T>::GetReaderCache() {
    return &_reader;
}

// 交换缓存，返回被移除的对象列表
template <class T>
inline std::list<T*> CacheRefresh<T>::Swap() {
    std::list<T*> rs;
    for (auto one : _add) {
        _reader.push_back(one);
    }
    _add.clear();

    for (auto one : _remove) {
        auto iterReader = std::find_if(_reader.begin(), _reader.end(), [one](auto x) {
            return x == one;
        });

        if (iterReader == _reader.end()) {
            std::cout << "CacheRefresh Swap error. not find obj to remove. sn:" << one->GetSN() << std::endl;
        } else {
            rs.push_back(one);
            _reader.erase(iterReader);
        }
    }
    _remove.clear();
    return rs;
}

// 判断是否可以交换缓存
template <class T>
inline bool CacheRefresh<T>::CanSwap() {
    return _add.size() > 0 || _remove.size() > 0;
}

// 将对象返回到池中
template<class T>
inline void CacheRefresh<T>::BackToPool() {
    for (auto iter = _add.begin(); iter != _add.end(); ++iter) {
        (*iter)->BackToPool();
    }
    _add.clear();

    for (auto iter = _remove.begin(); iter != _remove.end(); ++iter) {
        (*iter)->BackToPool();
    }
    _remove.clear();

    for (auto iter = _reader.begin(); iter != _reader.end(); ++iter) {
        (*iter)->BackToPool();
    }
    _reader.clear();
}
