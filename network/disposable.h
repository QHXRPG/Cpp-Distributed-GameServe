/* 
说明：所有网络服务类都继承此类 
功能：退出服务时统一清理资源
作者：QHXRPG
日期：2024/5/23
*/

class IDisposable
{
public:
    virtual ~IDisposable() = default;

    // 定义一个纯虚函数，要求子类必须重写
    virtual void Dispose() = 0;  
};