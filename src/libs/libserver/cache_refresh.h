#pragma once
#include <vector>
#include <algorithm>
#include <list>
#include <iostream>

// CacheRefreshģ���࣬���ڹ������Ļ���
template<class T>
class CacheRefresh {
public:
    std::vector<T*>* GetAddCache();		// ��ȡ��ӻ���
    std::vector<T*>* GetRemoveCache();	// ��ȡ�Ƴ�����
    std::vector<T*>* GetReaderCache();	// ��ȡ��ȡ����
    std::list<T*> Swap();				// �������棬���ر��Ƴ��Ķ����б�
    bool CanSwap();						// �ж��Ƿ���Խ�������
    void BackToPool();					// �����󷵻ص�����

protected:
    std::vector<T*> _reader;  // ��ȡ����
    std::vector<T*> _add;     // ��ӻ���
    std::vector<T*> _remove;  // �Ƴ�����
};

// ��ȡ��ӻ���
template <class T>
inline std::vector<T*>* CacheRefresh<T>::GetAddCache() {
    return &_add;
}

// ��ȡ�Ƴ�����
template <class T>
inline std::vector<T*>* CacheRefresh<T>::GetRemoveCache() {
    return &_remove;
}

// ��ȡ��ȡ����
template <class T>
inline std::vector<T*>* CacheRefresh<T>::GetReaderCache() {
    return &_reader;
}

// �������棬���ر��Ƴ��Ķ����б�
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

// �ж��Ƿ���Խ�������
template <class T>
inline bool CacheRefresh<T>::CanSwap() {
    return _add.size() > 0 || _remove.size() > 0;
}

// �����󷵻ص�����
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
