
#include <iostream>
#include <thread>
#include <memory>

// 防止子线程使用父线程释放的资源造成的崩溃问题
// 常见的作法
// 智能指针share_ptr(使用unique_ptr需要结合移动语言),全局变量，静态变量




// 线程函数的创建------》也就是线程执行函数
// 无参线程函数的调用
void test()
{
    std::cout << "test函数的调用" << std::endl;
}


// 有参线程的调用
void test2(std::shared_ptr<int>p1)
{
    std::cout << "test2函数的调用" << std::endl;
    std::cout << "子函数的使用:p1.use_count() = " << p1.use_count() << std::endl;
}

int main()
{
    // 创建智能指针对象
    std::shared_ptr<int> p1 = std::make_shared<int>(10);

    //std::thread func1(test);
   
    // 输出智能指针的对象个数
    std::cout << "输出前:p1.use_count() = " << p1.use_count() << std::endl;
    // 有参线程的调用
    std::thread func2(test2,p1);
    std::cout << "输出后:p1.use_count() = " << p1.use_count() << std::endl;
    
    // 主函数的输出
    std::cout << "main的调用" << std::endl;
    // 线程的销毁必须要一一对应
    //func1.detach();
    // 线程2的释放,与父线程分离,也就是说父线程不会等待子线程执行完毕，就会退出
    func2.detach();

    return 0;
}


// detach函数的作用--------->主线程与子线程分离
// 使用detach之后,主线程执行结束可以直接退出，不影响子线程的执行
// 需要注意的是,虽然做到主线程和子线程分离,如果子线程用到主线程回收的资源
// 仍然会导致主线程执行失败-------》这个地方可以使用share_ptr解决