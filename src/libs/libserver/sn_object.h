#pragma once

#include "common.h"
#include "global.h"

// SnObject 类
// 这是一个带有唯一序列号（SN）的基类。每个对象在创建时会自动生成一个唯一的序列号，
// 也可以通过构造函数传入自定义的序列号。提供了获取和重置序列号的方法。
class SnObject {
public:
    virtual ~SnObject() {}

    // 默认构造函数，自动生成一个唯一的序列号
    SnObject() {
        _sn = Global::GetInstance()->GenerateSN();
    }

    // 带参数的构造函数，使用传入的序列号
    SnObject(uint64 sn) {
        _sn = sn;
    }

    // 获取序列号
    uint64 GetSN() const {
        return _sn;
    }

    // 重置序列号，生成一个新的唯一序列号
    void ResetSN() {
        _sn = Global::GetInstance()->GenerateSN();
    }

protected:
    uint64 _sn;  // 序列号
};
