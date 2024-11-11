

#include<iostream>
#include<thread>


// 线程函数
void getThreadid()
{
    // 获取线程id
    // get_id()是一个静态成员函数,所以要使用这种作用域的方式获得
    std::thread::id tid = std::this_thread::get_id();
    std::cout << "线程id：" << tid << std::endl;
}


int main()
{
    std::thread func1(getThreadid);
    func1.join();
    // 主线程的线程id输出
    std::cout<<"主线程的线程id："<<std::this_thread::get_id()<<std::endl;
    return 0;
}