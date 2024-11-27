

// 简单版本的shared_ptr的实现

#include <iostream>
#include<atomic>


// 控制块的实现
template <typename T>
class ControlBlock
{
public:
    // 默认构造函数的实现
    // 只要创建这个控制块,引用计数最少为1
    ControlBlock():count_(1) {}
    // 增加引用计数
    void addRef() { ++count_; }
    // 减少引用计数
    void minusRef() { --count_; }   
    // 获取当前的引用计数
    int getCount() const { return count_; }
private:
    std::atomic<int> count_;
};

// 实现shared_ptr类
template <typename T>
class M_shared_ptr
{
public:
    // 默认构造函数
    // 指向控制块的指针要初始化，指向的数据指针也要初始化
    M_shared_ptr():ptr_(nullptr),controlBlock_(new ControlBlock<T>) {
        std::cout << "default constructor" << std::endl;
    }
    // 拷贝构造函数
    // 引用计数要变化，另外指向控制块的指针要变多
    M_shared_ptr(const M_shared_ptr &other):ptr_(other.ptr_),controlBlock_(other.controlBlock_)
    {
        // 拷贝构造函数的调用
        std::cout << "copy constructor"<<std::endl;
        // 增加引用计数
        controlBlock_->addRef();
    }

    // 赋值构造函数
    // 引用计数也会发生变化的
    M_shared_ptr &operator=(const M_shared_ptr &other)
    {
        // 除去自己赋值的情形
        if(this == &other)
        {
            return *this;
        }
        // 减少当前对象的引用计数
        controlBlock_->minusRef();
        if(controlBlock_->getCount() == 0){
            delete controlBlock_;;
            delete ptr_;
        }
        // 增加新对象的引用计数
        ptr_ = other.ptr_;
        controlBlock_ = other.controlBlock_;
        // 增加引用计数
        controlBlock_->addRef();
        return *this;
    }
    // 析构函数的使用
    ~M_shared_ptr()
    {
        // 析构函数的调用
        std::cout<<"~M_shared_ptr"<<std::endl;
        // 减少引用计数
        controlBlock_->minusRef();
        // 引用计数为0时，删除控制块
        if (controlBlock_->getCount() == 0)
        {
            delete controlBlock_;
            delete ptr_;
        }
    }
private:
    // 当前指向的指针
    T *ptr_;
public:
    // 引用计数的创建
    ControlBlock<T> *controlBlock_;
};


int main(){
    M_shared_ptr<int> ptr1; // 1
    // 输出引用计数
    std::cout<<ptr1.controlBlock_->getCount()<<std::endl; // 1
    M_shared_ptr<int> ptr2(ptr1);
    std::cout<<ptr2.controlBlock_->getCount()<<std::endl; // 2
    // // 赋值运算
    {
        M_shared_ptr<int> ptr3; // 1
        std::cout<<ptr3.controlBlock_->getCount()<<std::endl; // 1
        ptr3 = ptr2;
        std::cout<<ptr3.controlBlock_->getCount()<<std::endl; // 3
    }
    // ptr3出作用域就会被析构,那么引用计数就会变小
    std::cout<<ptr2.controlBlock_->getCount()<<std::endl; //2
    // ptr3 = ptr2; // 3
    // // 检查两人的引用计数变化
    // // 输出引用计数的变化
    // std::cout<<"-------------"<<std::endl;
    // // 最终三个指针指向一块区域
    // std::cout<<ptr3.controlBlock_->getCount()<<std::endl; //3
    // std::cout<<ptr2.controlBlock_->getCount()<<std::endl; //3
    // std::cout<<ptr1.controlBlock_->getCount()<<std::endl; //3
    return 0;
}