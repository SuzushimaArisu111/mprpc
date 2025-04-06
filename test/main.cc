#include "test.pb.h"
#include <iostream>
#include <string>
using namespace acid;

int main()
{
    LoginRequest req;
    req.set_name("zhang san");
    req.set_pwd("114514");

    // 对象数据序列化
    std::string send_str;
    if (req.SerializeToString(&send_str))
    {
        std::cout << send_str << std::endl;
    }

    // 从send_str反序列化一个login请求对象
    LoginRequest reqB;



    return 0;
}