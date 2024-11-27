
#include<iostream>

// 局部对象的返回
// 编译器会调用移动构造函数，如果没有移动构造函数再调用拷贝构造函数

class Test{
public:
    Test(){}
    // 拷贝构造函数
    Test(const Test &t){
        std::cout << "拷贝构造函数的调用"<<std::endl;
    }
    // 移动构造函数
    Test(Test && t){
        std::cout<<"移动构造函数的调用"<<std::endl;
    }
};

// 局部对象的返回
Test getTest(){
    Test t;
    return t;
}

int main(){
    getTest();
    return 0;
}