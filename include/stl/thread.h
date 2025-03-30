#pragma once
#include <pthread.h>
#include <iostream>
#include <tuple>
#include <utility>
#include "stl/tuple.h"
#include "template.h"

namespace stl {

    /*
     * 线程简化版1,只考虑基本数据类型
     */
    template<class T>
    class AssistedThread;

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
            //pthread_detach(tid);
            //runtime_init(this);
        };
    };


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

    /*
     * 线程简化版2,只考虑基本数据类型
     */


    template<class T, class... Args>
    class ThreadSimpleTemp {
    public:
        Tuple<Args...> args;
        T* fun;
        ThreadSimpleTemp(T* _fun, Args... _args) :fun(_fun) {
            args = Tuple<Args...>(_args...);
        };
    };

    template<typename>
    class ThreadTemp;

    template<int... Is>
    class ThreadTemp<IntList<Is...>> {
    public:
        template<class T, class... Args>
        static void* run(void* arg) {
            ThreadSimpleTemp<T, Args...> tem = *(ThreadSimpleTemp<T, Args...>*)arg;
            (tem.fun)(stl::TupleFindElement<Is>::find(tem.args.base)...);
        };
    };

    class ThreadSimple {
    public:
        pthread_t tid;
        template<class T, class... Args>
        ThreadSimple(T* _fun, Args... _args) {
            ThreadSimpleTemp<T, Args...>* temp = new  ThreadSimpleTemp<T, Args...>(_fun, _args...);
            static constexpr int size = sizeof...(Args) - 1;
            void* (*runtime_init)(void*) = &ThreadTemp<typename AssistedQueue<size>::QueueData>::template run<T, Args...>;
            pthread_create(&tid, nullptr, runtime_init, temp);
            pthread_detach(tid);
        };
    };



}