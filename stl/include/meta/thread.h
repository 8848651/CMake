// #pragma once
// #include <pthread.h>
// #include <iostream>
// #include <tuple>
// #include <utility>
// #include "tuple.h"
// #include "template.h"

// namespace stl {

//     template<class T, class... Args>
//     class ThreadSimpleTemp {
//     public:
//         Tuple<Args...> args;
//         T* fun;
//         ThreadSimpleTemp(T* _fun, Args... _args) :fun(_fun) {
//             args = Tuple<Args...>(_args...);
//         };
//     };

//     template<typename>
//     class ThreadTemp;

//     template<int... Is>
//     class ThreadTemp<IntList<Is...>> {
//     public:
//         template<class T, class... Args>
//         static void* run(void* arg) {
//             ThreadSimpleTemp<T, Args...> tem = *(ThreadSimpleTemp<T, Args...>*)arg;
//             (tem.fun)(stl::TupleFindElement<Is>::find(tem.args.base)...);
//         };
//     };

//     class thread {
//     public:
//         pthread_t tid;
//         template<class T, class... Args>
//         thread(T* _fun, Args... _args) {
//             ThreadSimpleTemp<T, Args...>* temp = new  ThreadSimpleTemp<T, Args...>(_fun, _args...);
//             static constexpr int size = sizeof...(Args) - 1;
//             //一个类方法如果是模版需要加上template
//             //静态  myclass::template fun<T> 非静态 myclass.template fun<T>
//             void* (*runtime_init)(void*) = &ThreadTemp<typename AssistedQueue<size>::QueueData>::template run<T, Args...>;
//             pthread_create(&tid, nullptr, runtime_init, temp);
//             pthread_detach(tid);
//         };

//         void join() {
//             int result = pthread_join(tid, nullptr);
//             if (result != 0) {
//                 throw std::runtime_error("Failed to join thread");
//             }
//         };
//     };



// }
