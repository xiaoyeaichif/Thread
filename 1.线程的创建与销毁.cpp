
#include <iostream>
#include <thread>

// 线程函数的创建------》也就是线程执行函数
// 无参线程函数的调用
void test()
{
    std::cout << "test函数的调用" << std::endl;
}


// 有参线程的调用
void test2(int value)
{
    std::cout << "test2函数的调用" << std::endl;
    std::cout << "value = " << value << std::endl;
}

int main()
{
    std::thread func1(test);
   
    // 有参线程的调用
    std::thread func2(test2,200);
    
    // 主函数的输出
    std::cout << "main的调用" << std::endl;
    // 线程的销毁必须要一一对应
    func1.join();
    // 线程2的释放
    func2.join();

    return 0;
}


// join函数的作用
// 主线程等待子线程执行完毕再结束，便于我们在控制台看到输出的结果