#include <iostream>
#include <map>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <cstring>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <pthread.h>
#include <vector>
#include <thread>
#include <functional>
#include <sys/syscall.h>
#include <meta/bind.h>

#include <iostream>
#include <functional> // 如果你的自定义 bind 在你的命名空间，替换为对应头
#include <stdexcept>
using namespace std;

// 你的可观察类型
struct ppt {
    int a = 0;
    ppt() { cout << "默认构造 a=" << a << "\n"; }
    ppt(int x): a(x) { cout << "带参构造 a=" << a << "\n"; }
    ppt(const ppt& pt) : a(pt.a + 1) {
        cout << "这是第 " << a << " 次拷贝\n";
    }
};

// 仅使用“全局函数”作为绑定目标
int add(int x, int y) {
    cout << "add(" << x << "," << y << ")\n";
    return x + y;
}

void set_to_sum(int& out, int x, int y) {
    out = x + y;
    cout << "set_to_sum: out=" << out << "\n";
}

// 用于观察按值/按引用/右值重载匹配
void sink(const ppt& x) { cout << "sink(const&): a=" << x.a << "\n"; }
void sink(ppt& x)       { cout << "sink(&): a=" << x.a << "\n"; }
void sink(ppt&& x)      { cout << "sink(&&): a=" << x.a << "\n"; }

void takes_ref(ppt& x) {
    cout << "takes_ref(&): a=" << x.a << "\n";
    x.a += 10;
}

void takes_by_value(ppt x) {
    cout << "takes_by_value: a=" << x.a << "（注意：形参会触发拷贝）\n";
}

int overloaded(int x) { cout << "overloaded(int)\n"; return x; }
double overloaded(double x) { cout << "overloaded(double)\n"; return x; }

void hello() { cout << "hello()\n"; }

int may_throw(int x) {
    if (x < 0) throw runtime_error("neg");
    return x;
}

int main() {

    cout << "=== test_basic_bind ===\n"<<endl;
    // 假设你的 bind 在命名空间 my（示例），请替换为你的名字空间
    auto add2 = stl::bind(add, 2, stl::placeholders::_1);
    //int r = add2(40);
    //cout << "result=" << r << "（期望 42）\n";

    // // 常量混合
    // auto add_10_32 = stl::bind(add, 10, 32);
    // cout << "add_10_32() = " << add_10_32() << "（期望 42）\n";




    return 0;
}
