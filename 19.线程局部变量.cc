


#include<iostream>
#include<thread>

// 线程局部变量的使用
thread_local int tlsVar = 0;

void Fun(int val)
{
    tlsVar = val;
    std::cout << "Thread ID: " <<std::this_thread::get_id()<<", tlsVar: " << tlsVar << std::endl;
}

// 主函数
int main(){
    // 生成两个线程，比较两个thread_local变量的值
    std::thread fun1(Fun,1);
    std::thread fun2(Fun,2);

    // 主函数的输出
    tlsVar = 3;
    std::cout << "Thread ID: " <<std::this_thread::get_id()<<", tlsVar: " << tlsVar << std::endl;

    // join
    fun1.join();
    fun2.join();
    return 0;
}

// 需要注意的是,线程是无顺序执行的,也就是说打印出来的顺序会有不同，这是需要注意！！！！
// 没有锁和条件变量的出现,顺序可能是乱序的,这是正常的现象