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




    template<class T>
    class AssistedThread;

    template<int... Is>
    class AssistedThread<IntList<Is...>> {
    public:
        template<class T, class... Args>
        static void* thread_runtime_init(void* arg) {
            Thread* tem = (Thread*)arg;
            Tuple<Args...>* args = (Tuple<Args...>*)(tem->arg);
            ((T*)(tem->fun))(stl::TupleFindElement<Is>::find(args->base)...);
            return nullptr;
        };

        template<class T, class... Args>
        static void* threadpool_runtime_init(void* arg) {
            Thread* tem = (Thread*)arg;
            Tuple<Args...>* args = (Tuple<Args...>*)(tem->arg);
            ((T*)(tem->fun))(stl::TupleFindElement<Is>::find(args->base)...);
            return nullptr;
        };
    };


    class Thread {
    public:
        void* fun = nullptr;
        void* arg = nullptr;
        void* (*runtime_init)(void*) = nullptr;
        pthread_t tid;

        template<class T, class... Args>
        Thread(T* _fun, Args... _args) {
            std::cout << "线程开启" << std::endl;
            static constexpr int size = sizeof...(Args) - 1;
            runtime_init = &AssistedThread<typename AssistedQueue<size>::QueueData>::template thread_runtime_init<T, Args...>;
            fun = (void*)_fun;
            arg = new Tuple<Args...>(_args...);
            pthread_create(&tid, nullptr, runtime_init, this);
            pthread_detach(tid);
            //runtime_init(this);
        };
    };


}