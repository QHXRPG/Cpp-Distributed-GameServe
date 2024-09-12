#pragma once

enum RobotStateType
{
    RobotState_HttpRequest = 0,		  // http����

    RobotState_Login_Connecting,	  // �������� Login
    RobotState_Login_Connected,	      // ���ӳɹ�
    RobotState_Login_Logined,         // ��¼�ɹ�

    //RobotState_Login_CreatePlayer,  // ������ɫ
    RobotState_Login_SelectPlayer,    // ѡ���ɫ

    RobotState_Game_Connecting,	      // �������� Game
    RobotState_Game_Connected,	      // ���ӳɹ�
    RobotState_Game_Logined,          // ��¼�ɹ�

    RobotState_Space_EnterWorld,      // �����ͼ

    RobotState_End,
};

const char RobotStateTypeName[][255] = {
    "[     ] HttpRequest :",
    "[Login] Connecting :",

    "[Login] Connected  :",
    "[Login] Logined     :",

    //"[Login] CreatePlayer:",
    "[Login] SelectPlayer:",

    "[Game ] Connecting :",
    "[Game ] Connected  :",
    "[Game ] Logined     :",

    "[Space] EnterWorld  :",
};

const char RobotStateTypeShortName[][255] = {
    "HttpRequest",

    "Login-Connecting",
    "Login-Connected",
    "Login-Logined",
    //"Login-CreatePlayer",
    "Login-SelectPlayer",

    "Game-Connecting",
    "Game-Connected",
    "Game-Logined",

    "Space-EnterWorld",
};

inline const char* GetRobotStateTypeShortName(RobotStateType stateType)
{
    if (stateType < 0 || stateType >= RobotState_End)
    {
        return "unknown";
    }

    return RobotStateTypeShortName[stateType];
}


inline const char* GetRobotStateTypeName(RobotStateType stateType)
{
    if (stateType < 0 || stateType >= RobotState_End)
    {
        return "unknown";
    }

    return RobotStateTypeName[stateType];
}