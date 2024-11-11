



// 互斥指的是,访问共享资源的时候,也就是我们常说的临界资源
// 需要互斥的访问,也就是说一个时刻只允许一个线程对这块内存进行修改
// 其他线程必须等待这个线程执行完毕才能对这块区域进行操作

// C++中实现互斥的方式有以下几种
// 1：原生的mutex,封装的锁：lock_guard,unique_lock,shared_lock

// 需要使用互斥的地方,多线程执行是必须考虑。

#include<iostream>
#include <thread>
#include <mutex>

// 生成一个共享变量(临界区资源)
static int num = 100;

// 设置一个全局锁
std::mutex mtx;

// 打印函数
void Output1()
{
    // 获取全局的一把锁
    // std::lock_guard<std::mutex> lock(mtx);
    while(num > 0)
    {
        // 放在这个地方,锁住的区域就更小
        std::lock_guard<std::mutex> lock(mtx);
        std::cout << "线程id1 = "<<std::this_thread::get_id()<<": "<<"num = "<< num <<std::endl;
        num--;
    }
}

void Output2()
{
    // 获取全局的一把锁
    //std::lock_guard<std::mutex> lock(mtx);
    while(num > 0)
    {
        std::lock_guard<std::mutex> lock(mtx);
        std::cout << "线程id2 = "<<std::this_thread::get_id()<<": "<<"num = "<< num <<std::endl;
        num--;
    }
}

// 主函数是的目的,两个线程函数轮流打印该线程的数据
int main()
{
    // 线程函数的绑定
    std::thread func1(Output1);
    // 第二个线程也打印这个函数
    std::thread func2(Output2);


    // 注意输出要写join
    func1.join();
    func2.join();   
    return 0;
}