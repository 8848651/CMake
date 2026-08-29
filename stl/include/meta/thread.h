#pragma once
#include <pthread.h>
#include <iostream>
#include <utility>
#include "tuple.h"
#include "template.h"
#include "indexqueue.h"

namespace stl {

    template<class T, class... Args>
    class threadsimpletemp {
    public:
        tuple<Args...> args;
        T* fun;
        threadsimpletemp(T* _fun, Args... _args) :fun(_fun) {
            args = Tuple<Args...>(_args...);
        };
    };

    template<typename>
    class threadtemp;

    template<int... Is>
    class threadtemp<indexqueue<Is...>> {
    public:
        template<class T, class... Args>
        static void* run(void* arg) {
            threadsimpletemp<T, Args...> tem = *(threadsimpletemp<T, Args...>*)arg;
            (tem.fun)(stl::tuplefindelement<Is>(tem.args)...);
        };
    };

    class thread {
    public:
        pthread_t tid;
        template<class T, class... Args>
        thread(T* _fun, Args... _args) {
            ThreadSimpleTemp<T, Args...>* temp = new  ThreadSimpleTemp<T, Args...>(_fun, _args...);
            static constexpr int size = sizeof...(Args) - 1;
            void* (*runtime_init)(void*) = &ThreadTemp<typename makeindexqueue<size>::queuedata>::template run<T, Args...>;
            pthread_create(&tid, nullptr, runtime_init, temp);
            pthread_detach(tid);
        };

        void join() {
            int result = pthread_join(tid, nullptr);
            if (result != 0) {
                throw std::runtime_error("Failed to join thread");
            }
        };
    };



}
