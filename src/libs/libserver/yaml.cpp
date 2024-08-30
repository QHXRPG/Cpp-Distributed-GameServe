#include "yaml.h"
#include "res_path.h"
#include "app_type_mgr.h"
#include "util_string.h"
#include "entity.h"
#include "entity_system.h"
#include "log4_help.h"

#include <tuple>

std::string DBMgrConfig::DBTypeMysql{ "mysql" };
std::string DBMgrConfig::DBTypeRedis{ "redis" };

Yaml::Yaml()
{
    auto pResPath = ResPath::GetInstance();
    if (pResPath == nullptr)
    {
        std::cout << "yaml awake failed. can't get ResPath." << std::endl;
        return;
    }

    const std::string path = pResPath->FindResPath("/engine.yaml");
    YAML::Node config = YAML::LoadFile(path);

    LoadConfig(APP_TYPE::APP_ALLINONE, config);
    LoadConfig(APP_TYPE::APP_LOGIN, config);
    LoadConfig(APP_TYPE::APP_ROBOT, config);
    LoadConfig(APP_TYPE::APP_DB_MGR, config);

    LOG_DEBUG("Yaml awake is Ok.");
}

YamlConfig* Yaml::GetConfig(const APP_TYPE appType)
{
    if (_configs.find(appType) != _configs.end())
    {
        return _configs[appType];
    }

    std::string appTypeName = AppTypeMgr::GetInstance()->GetAppName(appType);
    std::cout << "load config failed. appType:" << appTypeName.c_str() << std::endl;
    return nullptr;
}

void Yaml::LoadConfig(const APP_TYPE appType, YAML::Node& config)
{
    std::string appTypeName = AppTypeMgr::GetInstance()->GetAppName(appType);
    std::cout << "Loading config for app type: " << appTypeName << std::endl;

    AppConfig* pAppConfig = nullptr;

    try {
        switch (appType)
        {
        case APP_LOGIN:
        {
            auto pConfig = new LoginConfig();
            pConfig->UrlLogin = "http://192.168.137.68/member_login_t.php";
            pConfig->Ip = "127.0.0.1";
            pConfig->Port = 5401;
            pConfig->ThreadNum = 3;
            pAppConfig = pConfig;
            break;
        }
        case APP_DB_MGR:
        {
            auto pConfig = new DBMgrConfig();
            pConfig->Ip = "127.0.0.1";
            pConfig->Port = 5800;
            pConfig->ThreadNum = 1;

            // 创建DB配置
            DBConfig db1;
            db1.DBType = "redis";
            db1.Ip = "192.168.137.68";
            db1.Port = 6379;
            pConfig->DBs.push_back(db1);

            DBConfig db2;
            db2.DBType = "mysql";
            db2.Ip = "192.168.137.68";
            db2.Port = 3306;
            db2.User = "root";
            db2.Password = "qhxrpg";
            db2.DatabaseName = "account";
            db2.CharacterSet = "utf8";
            db2.Collation = "utf8_general_ci";
            pConfig->DBs.push_back(db2);

            pAppConfig = pConfig;
            break;
        }
        case APP_ROBOT:
        {
            auto pConfig = new RobotConfig();
            pConfig->ThreadNum = 5;
            pAppConfig = pConfig;
            break;
        }
        default:
        {
            auto pConfig = new CommonConfig();
            pConfig->Ip = "127.0.0.1";
            pConfig->Port = 5401;
            pConfig->ThreadNum = 5;
            pAppConfig = pConfig;
            break;
        }
        }

        _configs.insert(std::make_pair(appType, pAppConfig));
        std::cout << "Config loaded successfully for app type: " << appTypeName << std::endl;

    } catch (const std::exception &e) {
        std::cerr << "Error setting config for app type " << appTypeName << ": " << e.what() << std::endl;
        delete pAppConfig;
    }
}



DBConfig Yaml::LoadDbConfig(YAML::Node node) const
{
	DBConfig one;
	YAML::Node::iterator iter = node.begin();
	while (iter != node.end())
	{
		const std::string key = iter->first.as<std::string>();
		if (key == "type")
		{
			one.DBType = iter->second.as<std::string>();
			std::transform(one.DBType.begin(), one.DBType.end(), one.DBType.begin(), ::tolower);
		}

		else if (key == "ip")
			one.Ip = iter->second.as<std::string>();
		else if (key == "port")
			one.Port = iter->second.as<int>();
		else if (key == "user")
			one.User = iter->second.as<std::string>();
		else if (key == "password")
			one.Password = iter->second.as<std::string>();
		else if (key == "character_set")
			one.CharacterSet = iter->second.as<std::string>();
		else if (key == "collation")
			one.Collation = iter->second.as<std::string>();
		else if (key == "database_name")
			one.DatabaseName = iter->second.as<std::string>();

		++iter;
	}

	return one;
}

CommonConfig* Yaml::GetIPEndPoint(APP_TYPE appType, int appId)
{
    const auto pConfig = GetConfig(appType);
    const auto pListCfg = dynamic_cast<AppListConfig*>(pConfig);
    if (pListCfg != nullptr)
    {
        auto pOneCfg = pListCfg->GetOne(appId);
        if (pOneCfg == nullptr)
        {
            LOG_ERROR("can't find appType yaml's config. appType:" << appType << " id:" << appId);
            return nullptr;
        }

        return pOneCfg;
    }
    auto pAppConfig = dynamic_cast<CommonConfig*>(pConfig);
    if (pAppConfig == nullptr)
    {
        LOG_ERROR("can't find appType yaml's config. appType:" << appId);
        return nullptr;
    }

    return pAppConfig;
}
