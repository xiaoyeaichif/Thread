

// 简易线程池的实现
// 使用package_task和promise实现

#include <iostream>
#include <thread>
#include <future>
#include <mutex>
#include <condition_variable>
#include <atomic>
#include <vector>
#include <queue>

// 线程池的实现
/*
    线程池重必须有的参数
 */

const int TASK_MAX_THRESHHOLD   = 1024;//任务队列的最大数量
const int THREAD_MAX_THRESHHOLD = 100; //线程的上限
const int THREAD_MAX_IDLE_TIME  =  10; //单位:秒

class ThreadPool{
public:
    // 禁止拷贝构造函数和赋值函数
    // 线程池是一个单例类
    ThreadPool(const ThreadPool&) = delete;
    ThreadPool& operator=(const ThreadPool&) = delete;

    // 线程池的构造函数
    ThreadPool(unsigned int thread_num = 5):
    NumSize(thread_num),
    IsStart(false)
    {
        start();
    }

    // 析构函数
    ~ThreadPool(){
        stop();
    }

    // 当前活跃线程的数量
    size_t getActiveThreads() const{
        return NumSize;
    }
    // 空闲线程的数量
    size_t getFreeThreads() const{
        // 线程池的总大小减去活跃线程数字的大小
        //return Threads.size() - NumSize;
        return IdleNum;
    }

    // 线程池的启动
    void start(){};

    // 线程池的停止
    void stop(){};

    // 往线程池中添加任务

private:
    // 线程池的大小----》也就是线程初始化的数量
    std::atomic_int NumSize;
    // 线程池有没有启动
    std::atomic_bool IsStart;
    // 锁
    std::mutex mtx;
    // 条件变量
    std::condition_variable cv;
    // 存储线程的容器
    std::vector<std::thread> Threads;
    // 存储任务的队列
    using Task = std::packaged_task<void()>; // 生成一个函数模板,返回值为void的任务
    std::queue<Task> task_;

    // 空闲线程的数量
    std::atomic_int IdleNum;
};

int main(){
    return 0;
}
