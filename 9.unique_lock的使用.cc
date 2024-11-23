

// 判断线程是否持有锁
#include<iostream>
#include<thread>
#include<mutex>


// 全局锁
std::mutex mtx;
// 共享资源
int data = 0;

// 线程函数
void test()
{
    std::unique_lock<std::mutex> lock(mtx);
    data++;
    std::cout << "当前线程id:"<<std::this_thread::get_id()<<", data = " << data << std::endl;
    // 检查当前的线程是否拥有锁
    if(lock.owns_lock()){
        std::cout << "当前子线程拥有锁" << std::endl;
    }
    else{
        std::cout << "当前子线程未拥有锁" << std::endl;
    }
    // 暂时先释放锁
    std::cout<<"-------锁释放------"<<std::endl;
    lock.unlock();
    if(lock.owns_lock()){
        std::cout << "当前线程拥有锁" << std::endl;
    }
    else{
        std::cout << "当前线程未拥有锁" << std::endl;
    }
}


int main()
{
    // 主线程和子线程看谁先持有锁
    std::thread t1(test);
    std::this_thread::sleep_for(std::chrono::milliseconds(10)); // 延迟100毫秒
    // 主线程尝试抢锁
    {
        std::unique_lock<std::mutex> main_lock(mtx);
        if(main_lock.owns_lock()){
            std::cout << "主线程拥有锁" << std::endl;
            data++;
            std::cout << "主线程id:"<<std::this_thread::get_id()<<", data = " << data << std::endl;
        }
        else{
            std::cout << "主线程未拥有锁" << std::endl;
        }
    }
    t1.detach(); // 就行线程分离的形式
    return 0;
}