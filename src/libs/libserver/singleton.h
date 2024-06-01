#pragma once

#include<stdexcept>

template<class T>
class Singleton
{
private:
    static T* m_pInstance;

public:
    // ��������
    template<class... Args>
    static T* Instance(Args&&... args)
    {
        if(m_pInstance == nullptr)
        {
            m_pInstance = new T(std::forward<Args>(args)...);
        }
        return m_pInstance;
    }

    // ��ȡ����
    static T* GetInstance()
    {
        if(m_pInstance == nullptr)
        {
            throw std::logic_error("���ȳ�ʼ���������");
        }
        return m_pInstance;
    }

    // ɾ������
    static void DestroyInstance()
    {
        delete m_pInstance;
        m_pInstance = nullptr;
    }
};

// ��̬��Ա���������ⶨ��
template<class T> 
T* Singleton<T>::m_pInstance = nullptr;
