#pragma once
#include "tuple.h"
#include "construct.h"
#include "thread.h"
namespace stl {

    enum class placeholders {
        _1, _2, _3, _4, _5, _6, _7, _8, _9, _10
    };

    template<int N>
    class bind_assisted_helper {
        template<class... T, class... U>
        constexpr auto test(Tuple<T...>& _data, Tuple<U...>& _args) {
            constexpr int a = TupleFindType<N, placeholders>::find(_data.base);
            auto b = TupleFindElement<N>::find(_data.base);
            return b;
        };
    };

    template<class... Args>
    void tt(Args... args) {};


    template<class U>
    class bind_assisted;

    template<int... Is>
    class bind_assisted<IntList<Is...>> {
    public:
        template<class T, class... M, class... N>
        static void run(T* _func, Tuple<M...>& _data, Tuple<N...>& _args) {
            Tuple<decltype(bind_assisted_helper<Is>::test(_data, _args))...> _args_tmp;

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
    };

    template<class T, class... Args>
    auto bind(T* func, Args... args) {
        return anybind<T, Args...>(func, args...);
    };























}
