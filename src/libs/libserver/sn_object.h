#pragma once

#include "common.h"
#include "global.h"

// SnObject ��
// ����һ������Ψһ���кţ�SN���Ļ��ࡣÿ�������ڴ���ʱ���Զ�����һ��Ψһ�����кţ�
// Ҳ����ͨ�����캯�������Զ�������кš��ṩ�˻�ȡ���������кŵķ�����
class SnObject {
public:
    virtual ~SnObject() {}

    // Ĭ�Ϲ��캯�����Զ�����һ��Ψһ�����к�
    SnObject() {
        _sn = Global::GetInstance()->GenerateSN();
    }

    // �������Ĺ��캯����ʹ�ô�������к�
    SnObject(uint64 sn) {
        _sn = sn;
    }

    // ��ȡ���к�
    uint64 GetSN() const {
        return _sn;
    }

    // �������кţ�����һ���µ�Ψһ���к�
    void ResetSN() {
        _sn = Global::GetInstance()->GenerateSN();
    }

protected:
    uint64 _sn;  // ���к�
};
