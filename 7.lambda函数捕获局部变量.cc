


#include <iostream>
#include <memory>
#include <thread>

int main()
{
    // 生成一个局部变量
    // 使用一个lambda函数捕获这个局部变量的引用
    int a = 10;
    std::thread t([&a]()
    {
        std::cout << "a = " << a << std::endl;
    });
    // 如果使用detach，主线程会结束，子线程会继续执行，那么主线程回收资源,子线程使用
    // 主线程已经回收的资源，那么就会抛出异常，因此，这里使用join，主线程等待子线程执行完再结束
    // 或者使用shared_ptr来捕获子线程的资源
    // t.join();
    t.detach(); 
    // 主线程的输出
    std::cout << "a = " << a << std::endl;
    return 0;
}