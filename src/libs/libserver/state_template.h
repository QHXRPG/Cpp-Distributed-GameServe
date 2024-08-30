#pragma once

#include <map>

// ���崴����̬״̬�ĺ�
#define DynamicStateCreate(classname, enumType) \
    static void* CreateState() { return new classname; } \
    RobotStateType GetState() override { return enumType; }

// ����󶨶�̬״̬�ĺ�
#define DynamicStateBind(classname) \
    reinterpret_cast<CreateIstancePt>( &( classname::CreateState ) )

// T Ϊ����״̬����
template<typename enumType, class T>
class StateTemplate {
public:
    StateTemplate() {

    }

    // ���ø�����
    void SetParentObj(T* pObj) {
        _pParentObj = pObj;
    }

    virtual ~StateTemplate() { }

    // ��ȡ��ǰ״̬����
    virtual enumType GetState() = 0;
    
    // ����״̬���������µ�״̬����
    virtual enumType Update() = 0;

    // ����״̬ʱ�Ĳ���
    virtual void EnterState() = 0;
    
    // �뿪״̬ʱ�Ĳ���
    virtual void LeaveState() = 0;

protected:
    T* _pParentObj; // ������ָ��
};

template<typename enumType, class StateClass, class T>
class StateTemplateMgr {
public:
    virtual ~StateTemplateMgr() {
        if (_pState != nullptr) {
            delete _pState;
        }
    }

    // ��ʼ��״̬��������������Ĭ��״̬
    void InitStateTemplateMgr(enumType defaultState) {
        _defaultState = defaultState;
        RegisterState();
    }

    // �л�״̬
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

    // ���µ�ǰ״̬
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
    // ע��״̬�����麯������Ҫ����ʵ��
    virtual void RegisterState() = 0;

    ///////////////////////////////////////////////////////////////////////////////////
    // ����״̬��
public:
    typedef StateClass* (*CreateIstancePt)();

    // ����״̬����
    StateClass* CreateStateObj(enumType enumValue) {
        auto iter = _dynCreateMap.find(enumValue);
        if (iter == _dynCreateMap.end())
            return nullptr;

        CreateIstancePt np = iter->second;
        StateClass* pState = np();
        pState->SetParentObj(static_cast<T*>(this));
        return pState;
    }

    // ע��״̬��
    void RegisterStateClass(enumType enumValue, CreateIstancePt np) {
        _dynCreateMap[enumValue] = np;
    }

    ///////////////////////////////////////////////////////////////////////////////////

protected:
    std::map<enumType, CreateIstancePt> _dynCreateMap; // ��̬����ӳ���
    StateClass* _pState{ nullptr }; // ��ǰ״ָ̬��
    enumType _defaultState; // Ĭ��״̬
};