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



    template<class T>
    class ThreadWrapFun {
    public:
        T* fun;
        ThreadWrapFun(T* _fun) : fun(_fun) {};

    };

    template<class... T>
    class ThreadWrapArg {
    public:
        stl::Tuple<T...> args;
        ThreadWrapArg(T... _args) : args(_args...) {};


    };


    class Thread {
    public:
        void (*start_routine)(void*);
        void* fun;
        void* arg;

        template<class Function>
        Thread(Function* _fun) {
            start_routine = &Thread::runtime_init<Function>;
            fun = new ThreadWrapFun<Function>(_fun);
        };

        template<class Function, class... Args>
        Thread(Function* _fun, Args... args) {
            start_routine = &Thread::runtime_init<Function>;
            fun = new ThreadWrapFun<Function>(_fun);
            arg = new ThreadWrapArg<Args...>(args...);


        };

        //static无法访问对象属性 所以无法通过fun()调用
        template<class T>
        static void runtime_init(void* arg) {
            ThreadWrapFun<T>* wrap = (ThreadWrapFun<T>*)arg;
            (wrap->fun)();
        };

        void start() {
            start_routine(fun);
        }


    };


}