#pragma once
#include "tuple.h"
#include "construct.h"
#include "thread.h"
namespace stl {

    enum class placeholders {
        _1, _2, _3, _4, _5, _6, _7, _8, _9, _10
    };

    template<class T>
    class TT;

    template<int... Is>
    class TT<IntList<Is...>> {
    public:
        template<class... Args>
        static int run(Args... args) {
            static constexpr int size = sizeof...(Args) - 1;
            Tuple<Args...> _args(args...);
            int a = TupleFindType<4, placeholders>::find(_args.base);
            auto b = TupleFindElement<4>::find(_args.base);
            std::cout << "a=" << a << std::endl;
            std::cout << "b=" << b << std::endl;
            return 0;
        };
    };

    template<class... Args>
    void PP(Args... args) {
        static constexpr int size = sizeof...(Args) - 1;
        TT<class AssistedQueue<size>::QueueData>::run(args...);
    };





    template<typename>
    class BindTemp;

    class Bind;

    template<int... Is>
    class BindTemp<IntList<Is...>> {
    public:
        template<class T, class... Args>
        static void run(void* func) {
            Bind* bind = (Bind*)func;
            T* obj = (T*)(bind->method);
            Tuple<Args...> _args = *(Tuple<Args...>*)(bind->data);
            obj(TupleFindElement<Is>::find(_args.base)...);
        }
    };

    class Bind {
    public:
        void* _method = nullptr;
        void* _data = nullptr;
        void (*_func)(void*) = nullptr;

        void* data = nullptr;
        using FuncType = int;

        template<class T, class... Args>
        Bind(T* func, Args... args) {
            _method = (void*)func;
            _data = new Tuple<Args...>(args...);
            static constexpr int size = sizeof...(Args) - 1;
            _func = &BindTemp<typename AssistedQueue<size>::QueueData>::template run<T, Args...>;
        };

        template<class... Args>
        auto operator()(Args... args) {
            _func(this);
            data = new Tuple<Args...>(args...);
        };
    };






















}
