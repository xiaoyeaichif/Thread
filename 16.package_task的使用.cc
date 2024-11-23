

// 异步三剑客package_task的使用----》传递的函数对象
// 和promise一样都需要再开辟线程处理
// 这个用在线程池当中用的比较多,用来代表异步任务处理

#include <iostream>
#include <future>
#include <thread>

int add(int a,int b){
    return a + b;
}

int main(){
    // 传递的是函数对象
    std::packaged_task<int(int,int)>task(add);
    // 绑定package_task和future
    std::future<int>ans = task.get_future();

    // 使用一个线程来执行task
    std::thread func(std::move(task),10,20);
    // 输出结果
    std::cout << ans.get() << std::endl;
    func.join();
    return 0;
}
