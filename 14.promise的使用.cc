

// promise本质上和中转站差不多
// 可以开启一个线程往这个中专站写入数据,然后异步通知主线程来获取这个中转站的数据
// 一定要搭配future使用

#include <iostream>
#include <future>
#include <thread>
#include <string>

// 单独开辟一个线程,执行的数据写入promise中
void test(std::promise<std::string>p){
    p.set_value("yejie");
}

int main(){
    // 主函数生成promise和future建立联系
    std::promise<std::string>temp;
    // 与future建立关系
    std::future<std::string>func1 = temp.get_future();
    // 与线程建立连接
    std::thread t(test,std::move(temp));
    // 执行future获得线程操作的动作
    std::cout<<func1.get()<<std::endl;
    t.join();
    return 0;
}