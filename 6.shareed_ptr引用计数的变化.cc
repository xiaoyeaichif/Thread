

#include <iostream>
#include <memory>

// 测试函数检查引用计数的变化
// 引用和传值的方式
void test1(std::shared_ptr<int>p){
    std::cout << "test1() p.use_count() = " << p.use_count() << std::endl;
}

// 使用的是引用,那么引用计数始终和传递进来的值一致，不会发生变化
// 也就是说p的引用计数始终不会发生变化
void test2(std::shared_ptr<int>&p){

    std::cout << "test2() p.use_count() = " << p.use_count() << std::endl;
}

int main(){
    std::shared_ptr<int>p1 (new int);
    std::cout << "p1.use_count() = " << p1.use_count() << std::endl;
    // 引用计数进入这个test1函数会发生变化
    // 处test1作用域之后,引用计数恢复1
    test1(p1);
    // p1引用计数的大小为1
    test2(p1);
    return 0;
}