#pragma once

#include <memory>
#include <iostream>

class Widget {
public:
    Widget();
    ~Widget();
    void publicMethod();

private:
    struct Impl; 
    std::shared_ptr<Impl> pImpl;  
};

struct Widget::Impl {
    void privateMethod() {}
};

Widget::Widget() : pImpl(new Impl) {}

void Widget::publicMethod() {
    pImpl->privateMethod();  
}

//-----------------------------------------------------

class A : public std::enable_shared_from_this<A> {
public:
    std::shared_ptr<A> test() {
        return shared_from_this();
    };
};

void test_A() {
    std::shared_ptr<A> ptr = std::make_shared<A>();
    std::cout << ptr << std::endl;
    std::cout << ptr->test() << std::endl;
}
