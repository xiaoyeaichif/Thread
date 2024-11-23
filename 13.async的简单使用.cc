

#include<iostream>
#include<future>
#include<thread>

// async异步函数,相当于是thread的封装
// 之前thread对象不能够返回对象的值,async结合future可以做到

int add(int a,int b){
    // 输出当前线程的id
    std::cout<<"child thread id: "<<std::this_thread::get_id()<<std::endl;
    return a+b;
}

int main(){
    // 使用异步函数开辟线程执行add函数
    // async的返回值为future<int>
    // 默认的执行是开辟一个新线程执行
    // async(std::launch::async,add,1,2)
    // 还可以指定在一个线程当中执行
    // async(std::launch::deferred,add,1,2)
    std::future<int>func1 = std::async(add,1,2);
    // 使用get成员函数获得future<int>的返回值
    // 另外主线程会阻塞在async的get函数上，等待async所属线程执行完毕
    std::cout << func1.get() << std::endl;
    // 在一个线程当中执行---->在主线程当中执行
    std::future<int>func2 = std::async(std::launch::deferred,add,1,2);
    std::cout << func2.get() << std::endl;
    std::cout<<"father thread id: "<<std::this_thread::get_id()<<std::endl;
    return 0;
}