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
            return nullptr;
        };

        static void print() {
            //逗号运算符，展开参数列表
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
        //1:模式一
        // typedef typename AssistedQueue<Is - 1>::QueueData QueueType;
        // typedef typename Assemble<IntList<Is>, QueueType>::type QueueData;
        //2:模式二
        typedef AssistedQueue<Is - 1> QueueType;
        typedef typename Assemble<IntList<Is>, typename QueueType::QueueData>::type QueueData;
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
        pthread_t tid;

        template<class T, class... Args>
        Thread(T* _fun, Args... args) {
            static constexpr int size = sizeof...(Args) - 1;
            void* (*runtime_init)(void*) =                                   //template暂时还不清楚这个语法
                static_cast<void* (*)(void*)>(&AssistedQueue<size>::QueueData::template runtime_init<T, Args...>);
            fun = (void*)_fun;
            arg = new Tuple<Args...>(args...);
            pthread_create(&tid, nullptr, runtime_init, this);
            pthread_detach(tid);
            //runtime_init(this);
        };
    };


}