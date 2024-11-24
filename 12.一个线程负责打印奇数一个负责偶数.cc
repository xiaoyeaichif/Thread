

// 打印100以内的奇数和偶数
// 使用两个线程，一个线程只打印奇数，一个只打印偶数，交替打印

#include<iostream>
#include<mutex>
#include<thread>
#include<condition_variable>

// 打印100以内的奇数和偶数
int num = 1;
// 打印的次数
int count = 100; 
// 全局锁
std::mutex mtx;
// 条件变量
std::condition_variable condOdd;
std::condition_variable condEven;


// 负责打印奇数
void outOdd(){
    while(count > 0){
        // 抢锁
        std::unique_lock<std::mutex>lock(mtx);
        // 检查当前元素是不是奇数,如果是打印该元素
        // 检查当前元素是不是奇数,如果是，直接打印，并操作
        // 如果不是,释放锁资源，重新抢锁
        condOdd.wait(lock,[](){
            return num % 2 == 1;
        });
        // 一旦count == 0退出
        if(count == 0) break;
        // 满足是奇数之后
        std::cout<<std::this_thread::get_id()<<", odd: "<<num<<std::endl;
        // 打印的次数减少
        count--;
        // 数字增加
        num++;
        // 唤醒另外一个偶数线程
        condEven.notify_one();
    }
}

// 负责打印偶数
void outEven(){
    while(count > 0){
        // 抢锁
        std::unique_lock<std::mutex>lock(mtx);
        // 检查当前元素是不是偶数,如果是打印该元素
        // 检查当前元素是不是偶数,如果是，直接打印，并操作
        // 如果不是,释放锁资源，重新抢锁
        // 也就是说wait的第二个参数为true继续执行,为false就等待，并释放锁
        condEven.wait(lock,[](){
            return num % 2 == 0 ;
        });
        // 一旦count == 0退出
        if(count == 0) break;
        // 满足是奇数之后
        std::cout<<std::this_thread::get_id()<<", Even: "<<num<<std::endl;
        // 打印的次数减少
        count--;
        // 数字增加
        num++;
        // 唤醒另外一个奇数线程
        condOdd.notify_one();
    }
}

// 主线程
int main(){
    std::thread func1(outOdd);
    std::thread func2(outEven);
    func1.join();
    func2.join();
    return 0;
}