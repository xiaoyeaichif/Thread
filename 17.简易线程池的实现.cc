

// 简易线程池的实现
// 使用package_task和promise实现
// 线程池主要处理的任务都是作用差不多的,比如I/O计算等等,因为池子里的线程的状态是一致的

#include <iostream>
#include <thread>
#include <future>
#include <mutex>
#include <condition_variable>
#include <atomic>
#include <vector>
#include <queue>
#include <memory>
#include <functional>

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

    // 获取一个单例的对象
    static ThreadPool &getInstance(){
        // 线程安全的生成一个静态局部对象
        static ThreadPool ins;
        return ins;
    }

    // 线程池的构造函数
    // 初始化线程池的大小,如果默认的大小最少为1
    ThreadPool(unsigned int thread_num = 5): IsStart(false),active_threads_(0){
        if(thread_num < 1){
            thread_num = 1;
        }else{
            NumSize = thread_num;
        }
        start(); // 启动线程池
    }

    // 析构函数
    ~ThreadPool(){
        stop();
    }

    // 提交任务到线程池，并返回 std::future 以便获取结果
    template<typename F, typename... Args>
    // 返回的其实就是std::future<decltype(f(args...))>
    // 使用decltype可以获取函数返回值类型，也就是future<T>类型了
    // f指的就是回调函数,args...是可变参数包，也就是可变参数列表
    // 例如：f(int a,int b)->int,decltype(f(args...))就是int
    auto submitTask(F&& f, Args&&... args)->std::future<decltype(f(args...))>{
        // 使用别名，这个是返回值的推导
        using RetType = decltype(f(args...));
        // 检查线程池的状态
        if(IsStart.load())
        {
            return std::future<RetType>{};
        }
        // 生成任务
        // task 是一个 std::shared_ptr<std::packaged_task<RetType()>> 类型的智能指针，
        // 指向一个 std::packaged_task 对象。
        auto task = std::make_shared<std::packaged_task<RetType()>>(
            // 完美转发
            // 生成一个无参的函数，然后调用这个函数
            /*
                int add(int a,int b)
                {
                    return a+b;
                }
                下面的才做相当于
                func = bind(add,1,2);
                使用func()即可调用这个函数,并得到返回值
                // shared_ptr<int> p1 = make_shared<int>(4)
                // std::packaged_task<RetType()与
                std::bind(std::forward<F>(f), std::forward<Args>(args)...)的返回值类型相同
            */
            std::bind(std::forward<F>(f), std::forward<Args>(args)...)
        );
        // 绑定packaged_task和future
        std::future<RetType>result = task->get_future();
        // 抢锁，往队列中添加任务
        {
            std::unique_lock<std::mutex>lock(mtx);
            // *task 解引用智能指针，得到 std::packaged_task<RetType()> 对象本身。
            // (*task)(); 调用 std::packaged_task<RetType()>::operator() 方法，执行包装的任务
            task_.emplace([task](){ (*task)(); });
        }
        // 通知其他线程
        cv.notify_all();
        // 返回异步的结果
        return result;
    }


    // 当前活跃线程的数量
    size_t getActiveThreads() const{
        return NumSize;
    }
    // 空闲线程的数量
    size_t getFreeThreads() const{
        // 线程池的总大小减去活跃线程数字的大小
        return Threads.size() - active_threads_;
    }

    // 线程池的启动
    // 线程的创建
    void start(){
        for(size_t i = 0; i < NumSize; ++i){
            // 创建线程,并设置回调函数执行任务
            Threads.emplace_back([this](){
                // 判断当前的线程池是不是已经关闭
                // true:关闭，false:打开
                // load表示获取当前原子变量的状态
                while(!this->IsStart.load()){
                    // 用于从任务队列task_获取一个线程
                    Task task;
                    {
                        // 抢锁
                        std::unique_lock<std::mutex> lock(mtx);
                        // 判断是不是阻塞
                        // 如果IsStart.load() == false或者任务队列非空,需要执行任务
                        this->cv.wait(lock,[this]{
                            return this->IsStart.load() || !this->task_.empty();
                        });
                        // 判断是不是空
                        // 不用this也可以的
                        // 这里增加这个条件是防止仔IsStart.load() == false的时候，任务队列为空造成的错误
                        if(this->task_.empty()){
                            return;
                        }
                        // 如果任务队列不为空,从任务队列当中取出任务执行
                        // 获取任务队列的任务
                        task = std::move(this->task_.front());
                        this->task_.pop();
                    }
                    active_threads_++;  // 增加活动线程的数量
                    task(); // 执行任务
                    // 输出线程的状态
                    active_threads_--; // 减少活动线程的数量
                }
            });
            // 输出创建的线程的id
            std::cout<< "线程开始创建了...."<< Threads[i].get_id() <<std::endl;
        }
    }

    // 线程池的停止
    void stop(){
        // 这个作用域出去之后就会释放锁
        // 设置线程池为关闭状态
        {
            std::lock_guard<std::mutex>lock(mtx); 
            IsStart = true;
        }
        // 唤醒所有还在处理任务的线程继续执行任务
        // 唤醒所有的线程检查自己的状态，从而可以正常的退出，不用阻塞在条件变量上
        cv.notify_all();

        // 线程池的销毁
        for(auto& thread:Threads){
            // 避免重复join，造成死锁
            if(thread.joinable())
            {
                // 回收线程
                std::cout<<"thread join: "<<thread.get_id()<<std::endl;
                thread.join();
            }
        }
    }

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

    // 活动线程的数量
    std::atomic_int active_threads_;
};

int main() {
    ThreadPool &pool = ThreadPool::getInstance();
    auto future1 = pool.submitTask([](int a, int b) { return a + b; }, 1, 2);
    auto future2 = pool.submitTask([](int a, int b) { return a * b; }, 3, 4);
    auto future3 = pool.submitTask([](int a, int b) { return a + b; }, 1, 2);
    auto future4 = pool.submitTask([](int a, int b) { return a * b; }, 3, 4);
    auto future5 = pool.submitTask([](int a, int b) { return a + b; }, 1, 2);
    auto future6 = pool.submitTask([](int a, int b) { return a * b; }, 3, 4);

    std::cout << "Result of 1 + 2: " << future1.get() << std::endl;
    std::cout << "Result of 3 * 4: " << future2.get() << std::endl;
    std::cout << "Result of 1 + 2: " << future3.get() << std::endl;
    std::cout << "Result of 3 * 4: " << future4.get() << std::endl;
    std::cout << "Result of 1 + 2: " << future5.get() << std::endl;
    std::cout << "Result of 3 * 4: " << future6.get() << std::endl;
    std::cout << "Active threads: " << pool.getActiveThreads() << std::endl;
    std::cout << "Free threads:"<<pool.getFreeThreads()<<std::endl;

    return 0;
}
