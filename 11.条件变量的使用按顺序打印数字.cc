

// 结合条件变量和锁的功能打印数字
// 按需打印12121212121212这样顺序的数字

#include <iostream>
#include <thread>
#include <mutex>
#include <condition_variable>

int num = 1;
std::mutex mtx;
std::condition_variable conv1;
std::condition_variable conv2;

// 线程1只打印1这个数字
void test1(){
    while(true){
        // 抢锁
        std::unique_lock<std::mutex>lock(mtx);
        // 检查是否满足打印的条件
        // 如果当前的lambda不满足，则会释放锁，等待再次获取锁
        conv1.wait(lock,[](){
            return num == 1;
        });
        // 处理打印操作
        std::cout <<std::this_thread::get_id()<< " : " <<num << std::endl;
        num += 1;
        // 唤醒另一个线程
        conv2.notify_one();
    }
}

// 线程2只打印2这个数字
void test2(){
    while(true){
        // 抢锁
        std::unique_lock<std::mutex>lock(mtx);
        // 检查是否满足打印的条件
        conv2.wait(lock,[](){
            return num == 2;
        });
        // 处理打印操作
        std::cout <<std::this_thread::get_id()<< " : " <<num << std::endl;
        num -= 1;
        // 唤醒另一个线程
        conv1.notify_one();
    }
}




int main(){
    // 处理两个线程
    std::thread func1(test1),func2(test2);
    func1.join();
    func2.join();
    return 0;
}