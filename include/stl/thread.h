#pragma once
#include <pthread.h>
#include <iostream>
#include <tuple>
#include <utility>
#include "stl/tuple.h"

namespace stl {

    class Thread;


    template<int Is>
    class IntOne;

    template<int... Is>
    class IntList;

    template<int... Is>
    class IntList {
    public:

        template<class T, class... Args>
        static void* runtime_init(void* arg) {
            static constexpr int size = sizeof...(Args);
            Thread* tem = (Thread*)arg;
            Tuple<Args...>* args = (Tuple<Args...>*)(tem->arg);
            ((T*)(tem->fun))();
            //TODO: 明天实现对参数的展开每一项的处理方式
            // std::get<Is>(args->base)...


        };

        static void print() {
            // 最典型std::get<Is>(tuple)... 参数展开
            // 对Is中的每个元素，调用print_helper(i)
            (print_helper(Is), ...);
        }
    private:
        static void print_helper(int i) {
            std::cout << i << " ";
        }
    };


    template<typename T, typename U>
    class Assemble;

    template<int First, int... Rest>
    class Assemble<IntOne<First>, IntList<Rest...>> {
    public:
        typedef IntList<Rest..., First> type;
    };

    template<int Is>
    class AssistedQueue {
    public:
        typedef typename AssistedQueue<Is - 1>::QueueData QueueType;
        typedef typename Assemble<IntOne<Is>, QueueType>::type QueueData;
    };

    template<>
    class AssistedQueue<0> {
    public:
        typedef IntList<0> QueueData;
    };


    class Thread {
    public:
        void* fun;
        void* arg;

        template<class T, class... Args>
        Thread(T* _fun, Args... args) {
            static constexpr int size = sizeof...(Args) - 1;
            void* (*runtime_init)(void*) =                                   //template暂时还不清楚这个语法
                static_cast<void* (*)(void*)>(&AssistedQueue<size>::QueueData::template runtime_init<T, Args...>);
            fun = (void*)_fun;
            arg = new Tuple<Args...>(args...);
            runtime_init(this);
        };
    };


}