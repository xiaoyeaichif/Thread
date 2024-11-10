
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
    std::thread func2(test2,100);
    
    // 主函数的输出
    std::cout << "main的调用" << std::endl;
    // 线程的销毁必须要一一对应
    func1.detach();
    // 线程2的释放
    func2.detach();

    return 0;
}


// detach函数的作用--------->主线程与子线程分离
// 使用detach之后,主线程执行结束可以直接退出，不影响子线程的执行
// 需要注意的是,虽然做到主线程和子线程分离,如果子线程用到主线程回收的资源
// 仍然会导致主线程执行失败-------》这个地方可以使用share_ptr解决