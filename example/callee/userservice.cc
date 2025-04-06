#include<iostream>
#include<string>
#include"../user.pb.h"
#include "mprpcapplication.h"
#include "rpcprovider.h"

/*

*/
class UserService :public acid::UserServiceRpc // 使用在rpc服务端
{
public:
    bool Login(std::string name, std::string pwd)
    {
        std::cout << "doing local service:Login" << std::endl;
        std::cout << "name:" << name << " pwd:" << pwd << std::endl;
        return false;
    }

    bool Register(uint32_t id, std::string name, std::string pwd)
    {
        std::cout << "doing local service:Register" << std::endl;
        std::cout << "name:" << name << " pwd:" << pwd << std::endl;
        return true;
    }

    // 重写基类UserServiceRpc的虚函数 下面这些方法都是框架直接调用的
    // 1 caller ==> Login ==> muduo ==> callee
    // 2 callee ==> Login ==> 交到重写的方法上
    void Login(::google::protobuf::RpcController* controller,
        const ::acid::LoginRequest* request,
        ::acid::LoginResponse* response,
        ::google::protobuf::Closure* done)
    {
        // 框架给业务上报了请求参数LoginRequest，应用获取相应数据做本地业务
        std::string name = request->name();
        std::string pwd = request->pwd();

        // 做本地业务
        bool login_result = Login(name, pwd);

        // 把响应写入 包括错误码 错误消息 返回值
        acid::ResultCode* code = response->mutable_result();
        code->set_errcode(1);
        code->set_errmsg("Login do error!");
        response->set_sucess(login_result);

        // 执行回调操作 执行响应对象数据的序列化和网络发送（由框架完成）
        done->Run();
    }

    void Register(::google::protobuf::RpcController* controller,
        const ::acid::RegisterRequest* request,
        ::acid::RegisterResponse* response,
        ::google::protobuf::Closure* done)
    {
        uint32_t id = request->id();
        std::string name = request->name();
        std::string pwd = request->pwd();

        bool ret = Register(id, name, pwd);

        response->mutable_result()->set_errcode(0);
        response->mutable_result()->set_errmsg("");
        response->set_sucess(ret);

        done->Run();
    }
};

int main(int argc, char** argv)
{
    // 调用框架的初始化操作
    MprpcApplication::Init(argc, argv);

    // 把userService对象发布到rpc节点上
    RpcProvider provider;
    provider.NotifyService(new UserService());

    // 启动一个rpc服务发布节点
    provider.Run();

    return 0;
}