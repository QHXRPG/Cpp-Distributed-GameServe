#pragma once

#include<stdexcept>

template<class T>
class Singleton
{
private:
    static T* m_pInstance;

public:
    // 创建单例
    template<class... Args>
    static T* Instance(Args&&... args)
    {
        if(m_pInstance == nullptr)
        {
            m_pInstance = new T(std::forward<Args>(args)...);
        }
        return m_pInstance;
    }

    // 获取单例
    static T* GetInstance()
    {
        if(m_pInstance == nullptr)
        {
            throw std::logic_error("请先初始化这个单例");
        }
        return m_pInstance;
    }

    // 删除单例
    static void DestroyInstance()
    {
        delete m_pInstance;
        m_pInstance = nullptr;
    }
};

// 静态成员变量在类外定义
template<class T> 
T* Singleton<T>::m_pInstance = nullptr;
