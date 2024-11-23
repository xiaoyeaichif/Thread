

// promise本质上和中转站差不多
// 可以开启一个线程往这个中专站写入数据,然后异步通知主线程来获取这个中转站的数据
// 一定要搭配future使用

#include <iostream>
#include <future>
#include <thread>
#include <string>
#include <chrono>
#include <vector>

// 单独开辟一个线程,执行的数据写入promise中
void test(std::promise<long long>p,long long left,long long right){
    long long  sum = 0;
    for(long long i = left;i < right;i++)
    {
        sum += i;
    }
    p.set_value(sum);
}

int main(){
    // 主函数生成promise和future建立联系
    std::promise<long long>temp;
    // 与future建立关系
    std::future<long long>func1 = temp.get_future();

    // 统计执行的时间
    auto start = std::chrono::high_resolution_clock::now();
    long long left = 0,right = 99999999;
    // 与线程建立连接
    std::thread t(test,std::move(temp),left,right);

    int result = func1.get();
    
    // 记录结束时间
    auto end = std::chrono::high_resolution_clock::now();

    // 计算并输出执行时间
    std::chrono::duration<double> duration = end - start;
    std::cout << "异步执行加法运算的时间: " << duration.count() << " 秒" << std::endl;

    // 输出结果
    std::cout << "结果: " << result << std::endl;

    // 开辟四个异步线程来执行加法操作
    // 每个线程操作一部分,检查时间消耗多少
    const int N = 4;
    {
        std::vector<std::future<long long>> results(N);
        std::vector<std::promise<long long>>Promises(N);
        std::vector<std::thread>threads(N);
    
        // 计算时间
        auto start = std::chrono::high_resolution_clock::now();
        // 钩子（promise）与future建立连接
        for(int i = 0;i < N;i++)
        {
            results[i] = Promises[i].get_future();
            // 边界条件的改变
            long long Begin = (right - left + 1) / N * i ;
            long long End = (right - left + 1) / N * (i + 1) - 1;
            threads[i] =std::thread(test,std::move(Promises[i]),Begin,End);
        }
        // 执行总和的计算
        long long allsum = 0;
        for(int i = 0;i < N;i++)
        {
            // 每一个函数的累加
            allsum += results[i].get();
        }

        // 时间的结束
        auto end = std::chrono::high_resolution_clock::now();

        // 计算并输出执行时间
        std::chrono::duration<double> duration = end - start;
        std::cout << "异步执行加法运算的时间: " << duration.count() << " 秒" << std::endl;


        // 线程的处理
        for(int i = 0;i < 4;i++)
        {
            // 线程的结束
            threads[i].join();
        }
    }

    t.join();
    return 0;
}