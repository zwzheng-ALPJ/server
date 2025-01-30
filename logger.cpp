#include "logger.h"

logger& logging()
{
    static logger Logger; //静态局部变量确保单例模式
    return Logger;
}

logger::~logger()
{
    //析构时打印出缓冲区内容
    std::cout<<std::endl;
}
