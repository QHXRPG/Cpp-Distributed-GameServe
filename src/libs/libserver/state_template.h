#pragma once

#include <map>

// 定义创建动态状态的宏
#define DynamicStateCreate(classname, enumType) \
    static void* CreateState() { return new classname; } \
    RobotStateType GetState() override { return enumType; }

// 定义绑定动态状态的宏
#define DynamicStateBind(classname) \
    reinterpret_cast<CreateIstancePt>( &( classname::CreateState ) )

// T 为管理状态的类
template<typename enumType, class T>
class StateTemplate {
public:
    StateTemplate() {

    }

    // 设置父对象
    void SetParentObj(T* pObj) {
        _pParentObj = pObj;
    }

    virtual ~StateTemplate() { }

    // 获取当前状态类型
    virtual enumType GetState() = 0;
    
    // 更新状态，并返回新的状态类型
    virtual enumType Update() = 0;

    // 进入状态时的操作
    virtual void EnterState() = 0;
    
    // 离开状态时的操作
    virtual void LeaveState() = 0;

protected:
    T* _pParentObj; // 父对象指针
};

template<typename enumType, class StateClass, class T>
class StateTemplateMgr {
public:
    virtual ~StateTemplateMgr() {
        if (_pState != nullptr) {
            delete _pState;
        }
    }

    // 初始化状态管理器，并设置默认状态
    void InitStateTemplateMgr(enumType defaultState) {
        _defaultState = defaultState;
        RegisterState();
    }

    // 切换状态
    void ChangeState(enumType stateType) {
        StateClass* pNewState = CreateStateObj(stateType);
        if (pNewState == nullptr) {
            //LOG_ERROR( "ChangeState:" << stateType << " == nullptr" );
            return;
        }

        if (pNewState != nullptr) {
            if (_pState != nullptr) {
                _pState->LeaveState();
                delete _pState;
            }

            _pState = pNewState;
            _pState->EnterState();
        }
    }

    // 更新当前状态
    void UpdateState() {
        if (_pState == nullptr) {
            ChangeState(_defaultState);
        }
        enumType curState = _pState->Update();
        if (curState != _pState->GetState()) {
            ChangeState(curState);
        }
    }

protected:
    // 注册状态，纯虚函数，需要子类实现
    virtual void RegisterState() = 0;

    ///////////////////////////////////////////////////////////////////////////////////
    // 创建状态类
public:
    typedef StateClass* (*CreateIstancePt)();

    // 创建状态对象
    StateClass* CreateStateObj(enumType enumValue) {
        auto iter = _dynCreateMap.find(enumValue);
        if (iter == _dynCreateMap.end())
            return nullptr;

        CreateIstancePt np = iter->second;
        StateClass* pState = np();
        pState->SetParentObj(static_cast<T*>(this));
        return pState;
    }

    // 注册状态类
    void RegisterStateClass(enumType enumValue, CreateIstancePt np) {
        _dynCreateMap[enumValue] = np;
    }

    ///////////////////////////////////////////////////////////////////////////////////

protected:
    std::map<enumType, CreateIstancePt> _dynCreateMap; // 动态创建映射表
    StateClass* _pState{ nullptr }; // 当前状态指针
    enumType _defaultState; // 默认状态
};