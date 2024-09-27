#pragma once
#include <pthread.h>
#include <iostream>
#include <tuple>
#include <utility>
#include "stl/tuple.h"

namespace stl {




    template<int Is>
    class IntOne;

    template<int... Is>
    class IntList;

    template<int... Is>
    class IntList {
    public:
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
        void* (*start_routine)(void*);
        void* fun;
        void* arg;

        template<class Function, class... Args>
        Thread(Function* _fun, Args... args) {
            //start_routine = &Thread::runtime_init<Function, Args...>;
            fun = (void*)_fun;
            arg=new Tuple<Args...>(args...);
        };

        template<class T, class... Args>
        static void* runtime_init(void* arg) {
            Thread* tem = (Thread*)arg;
            ((T*)(tem->fun))();
            Tuple<Args...>* args = (Tuple<Args...>*)tem->arg;
        };


    };


}