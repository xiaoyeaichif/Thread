

#include <iostream>
#include <mutex>
#include <memory>

/*
** 单例模式的：懒汉式单例模式，饿汉式单例模式
** 饿汉式单例模式: 线程安全，但是在主线程创建之前就已经生成完毕了,所以是线程安全的
                但是在多线程的情况下，可能造成浪费，因为没有用到就浪费了
** 懒汉式的单例模式：在C++11中使用静态局部变量的形式来保证线程安全
*/
class SingLeton1{
public:
    // 获得私有的静态对象
    static SingLeton1 *getInstance(){
        return &instance;
    }
    SingLeton1(){
        std::cout << "饿汉式---构造函数的调用" << std::endl;
    }
    ~SingLeton1(){
        std::cout << "饿汉式---析构函数的调用" << std::endl;
    }
private:
    // 禁止拷贝构造和赋值函数
    SingLeton1(const SingLeton1&) = delete;
    // 拷贝赋值的禁止
    SingLeton1& operator=(const SingLeton1&) = delete;
    // 生成私有对象
    static SingLeton1 instance;
};
// 类外初始化
// 在SingLeton1作用域下生成一个instance对象
SingLeton1 SingLeton1::instance;


// 懒汉式实现单例模式
class SingLeton2{
public:
    // 返回静态局部变量
    static SingLeton2& getInstance(){
        static SingLeton2 instance;
        return instance;
    }
private:
    // 禁止拷贝和赋值函数
    SingLeton2(const SingLeton2&) = delete;
    SingLeton2& operator=(const SingLeton2&) = delete;
    // 私有构造函数
    SingLeton2(){
        std::cout << "懒汉式---构造函数的调用" << std::endl;
    }
    ~SingLeton2(){
        std::cout << "懒汉式---析构函数的调用" << std::endl;
    }
    // 静态成员函数，返回私有对象
};

// 懒汉式的第二种写法
// 锁 + 双重判断的形式
std::mutex mtx;

class SingLeton3{
public:
    // 获取单例对象
    // 锁 + 双重判断的逻辑
    static SingLeton3* getInstance(){
        if(pInstance == nullptr){
            std::lock_guard<std::mutex>lock(mtx);
            if(pInstance == nullptr){
                // 并不知道在何处释放内存
                // 所以使用智能指针的形式来开辟这片内存
                //pInstance = new SingLeton3();
                pInstance = std::make_unique<SingLeton3>();
            }
        }
        // 获取实际的对象SingLeton3
        return pInstance.get();
    }
public:
    // 实际instance类型为unique_ptr<SingLeton3>，需要显示获取SingLeton3
    static std::unique_ptr<SingLeton3> pInstance;
    // 禁止拷贝构造函数和赋值函数
    SingLeton3& operator=(const SingLeton3&) = delete;
    SingLeton3(const SingLeton3&) = delete;
    // 私有构造函数
    SingLeton3(){
        std::cout << "懒汉式2---构造函数的调用" << std::endl;
    }
    ~SingLeton3(){
        std::cout << "懒汉式2---析构函数的调用" << std::endl;
    }
};
// 类外初始化
// 生成智能指针管理的对象
// 实际instance类型为unique_ptr<SingLeton3>，需要显示获取SingLeton3
std::unique_ptr<SingLeton3> SingLeton3::pInstance;

int main(){
    // 函数的调用需要注意，只有一个对象被正确的调用
    SingLeton1 *p1 = SingLeton1::getInstance();
    // 引用的方式
    SingLeton2 &p2 = SingLeton2::getInstance();
    // 指针的形式
    SingLeton3 *p3 = SingLeton3::getInstance();
    // delete p3;
    return 0;
}