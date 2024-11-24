#include <iostream>
#include <thread>
#include <mutex>
#include <condition_variable>

std::mutex mtx;
std::condition_variable cv;
bool ready = false;

void worker_thread() {
    std::unique_lock<std::mutex> lock(mtx);
    // 使用 wait 和 lambda 表达式检查条件
    cv.wait(lock, []{ return ready; });
    // 条件满足后继续执行
    std::cout << "Worker thread is processing data! \n";
}

int main() {
    std::thread worker(worker_thread);

    // 主线程做一些准备工作
    std::this_thread::sleep_for(std::chrono::seconds(1));
    // 主线程的操作
    std::cout << "Main thread is processing data! \n";

    // 主线程没有改变ready,子线程抢到锁资源执行也没用,会被阻塞,然后又会被主线程抢到锁资源
    // 设置标志位并在同一临界区发送通知
    {
        std::lock_guard<std::mutex> lock(mtx);
        ready = true;
        cv.notify_one();
    }

    worker.join();

    return 0;
}






