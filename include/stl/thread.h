#pragma once
#include <pthread.h>
#include <iostream>
#include <tuple>
#include <utility>
#include "stl/tuple.h"

namespace stl {

    class Thread;

    template<int... Is>
    class IntList;

    template<int... Is>
    class IntList {
    public:

        template<class T, class... Args>
        static void* runtime_init(void* arg) {
            Thread* tem = (Thread*)arg;
            Tuple<Args...>* args = (Tuple<Args...>*)(tem->arg);
            ((T*)(tem->fun))(stl::TupleFindElement<Is>::find(args->base)...);
            // function<Is>(args)...
            //1:(stl::TupleFindElement<Is>::find(args->base),...)
            //2:(stl::TupleFindElement<Is>::find(args->base)...)
            //1和2不一样,注意这个逗号
        };

        static void print() {
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
    class Assemble<IntList<First>, IntList<Rest...>> {
    public:
        typedef IntList<Rest..., First> type;
    };

    template<int Is>
    class AssistedQueue {
    public:
        typedef typename AssistedQueue<Is - 1>::QueueData QueueType;
        typedef typename Assemble<IntList<Is>, QueueType>::type QueueData;
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