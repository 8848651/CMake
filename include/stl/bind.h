#pragma once
#include <iostream>
#include "tuple.h"
#include "construct.h"
#include "thread.h"
namespace stl {

    //需要注意的点
    //1:bind是函数不是类，如果是类类类型和方法类型无法区分
    //2:注意在方法里参数展开是从右到左展开，执行也是从右到左
    //3:constexpr类型初始化后无法改变的，可以通过is_nmber_minus将数转为类型，再将类型转为数实现
    //4:在使用三目运算符时后面两个类型必须是相同的，否则会出现编译错误，可以使用if constexpr,C++11没有可以使用ConditionalDispatcher代替

    enum class placeholders {
        _1, _2, _3, _4, _5, _6, _7, _8, _9, _10
    };

    std::ostream& operator<<(std::ostream& os, placeholders ph) {
        return os << (static_cast<int>(ph) + 1);
    }

    template <bool Flag, typename T, typename U>
    struct ConditionalDispatcher;

    // 当Flag为true时的特化版本
    template <typename T, typename U>
    struct ConditionalDispatcher<true, T, U> {
        static auto execute(T t, U u) -> decltype(t) { return t; }
    };

    // 当Flag为false时的特化版本
    template <typename T, typename U>
    struct ConditionalDispatcher<false, T, U> {
        static auto execute(T t, U u) -> decltype(u) { return u; }
    };

    template<int N>
    class bind_assisted_helper {
    public:

        template<class... T, class... U>
        constexpr static auto _test(Tuple<T...>& _data, Tuple<U...>& _args) {
            constexpr int a = TupleFindType<N, placeholders>::find(_data.base);
            auto b = TupleFindElement<N>::find(_data.base);
            //必须保证stl::is_nmber_minus<a, 1>::value>=0
            auto c = TupleFindElement<stl::is_nmber_minus<a, 1>::value>::find(_args.base);
            constexpr bool flag = stl::is_same<decltype(b), placeholders>::value ? true : false;
            return ConditionalDispatcher<flag, decltype(c), decltype(b)>::execute(c, b);
        };

    };


    template<class U>
    class bind_assisted;

    template<int... Is>
    class bind_assisted<IntList<Is...>> {
    public:
        template<class T, class... M, class... N>
        static void run(T* _func, Tuple<M...>& _data, Tuple<N...>& _args) {
            Tuple<decltype(bind_assisted_helper<Is>::_test(_data, _args))...> _args_tmp(bind_assisted_helper<Is>::_test(_data, _args)...);
            _func(TupleFindElement<Is>::find(_args_tmp.base)...);
        };

        template<class T, class... M>
        static void run(T* _func, Tuple<M...>& _data) {
            _func(TupleFindElement<Is>::find(_data.base)...);
        };


    };

    template<class T, class... Args>
    class anybind {
    public:
        T* _func;
        Tuple<Args...> _data;
        static constexpr int _size = sizeof...(Args) - 1;
        anybind(T* func, Args... args) : _func(func), _data(args...) {};

        template<class... U>
        auto operator()(U... args) {
            Tuple<U...> _args(args...);
            bind_assisted<class AssistedQueue<_size>::QueueData>::run(_func, _data, _args);
        };

        auto operator()() {
            bind_assisted<class AssistedQueue<_size>::QueueData>::run(_func, _data);
        };
    };

    template<class T, class... Args>
    auto bind(T* func, Args... args) {
        return anybind<T, Args...>(func, args...);
    };























}
