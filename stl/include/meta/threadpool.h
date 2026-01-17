// #pragma once
// #include <stddef.h>
// #include <pthread.h>
// #include <vector>
// #include <mutex>
// #include "template.h"
// #include "queue.h"

// namespace stl {

//     class ThreadPoolAssisted {
//     public:
//         void* fun;
//         void* arg;
//         //该函数要求传入Thread类型参数，即必须有函数指针fun Tuple<Args...>类型arg
//         void* (*runtime_init)(void*) = nullptr;

//         template<class T, class... Args>
//         ThreadPoolAssisted(T* _fun, Args... _args) {
//             static constexpr int size = sizeof...(Args) - 1;
//             runtime_init = &AssistedThread<typename AssistedQueue<size>::QueueData>::template threadpool_runtime_init<T, Args...>;
//             fun = (void*)_fun;
//             arg = new Tuple<Args...>(_args...);
//         };
//         //拷贝构造函数
//         ThreadPoolAssisted(const ThreadPoolAssisted& other) {
//             fun = other.fun;
//             arg = other.arg;
//             runtime_init = other.runtime_init;
//         };

//     public:
//         void start() {
//             runtime_init((void*)this);
//         }
//     };



//     template<typename>
//     class ThreadPoolTemp;

//     template<class T, class... Args>
//     class ThreadPoolSimple;

//     template<int... Is>
//     class ThreadPoolTemp<IntList<Is...>> {
//     public:
//         template<class T, class... Args>
//         static void* run(void* arg) {
//             ThreadPoolSimple<T, Args...> tem = *(ThreadPoolSimple<T, Args...>*)arg;
//             (tem.fun)(stl::TupleFindElement<Is>::find(tem.args.base)...);
//         };
//     };

//     template<class T, class... Args>
//     class ThreadPoolSimple {
//     public:
//         T* fun;
//         Tuple<Args...> arg;
//         ThreadPoolSimple(T* _fun, Args... _args): fun(_fun), arg(_args...) {};

//     public:
//         void start() {
//             static constexpr int size = sizeof...(Args) - 1;
//             ThreadPoolTemp<typename AssistedQueue<size>::QueueData>::template run<T, Args...>(this);  
//         }
//     };

//     class ThreadPool {
//     public:
//         //一个线程池两个线程 后续实现自定义线程数
//         int num_threads = 2;
//         pthread_t tid_1;
//         pthread_t tid_2;
//         stl::queue<ThreadPoolAssisted> Assisted;
//         ThreadPool() {
//             //std::cout << "线程池开启" << std::endl;
//             pthread_create(&tid_1, nullptr, runtime_init, this);
//             //pthread_create(&tid_2, nullptr, runtime_init, this);
//             pthread_detach(tid_1);
//         };

//         static void* runtime_init(void* arg) {
//             ThreadPool* pool = static_cast<ThreadPool*>(arg);
//             while (true) {
//                 // 在vector中取出任务
//                 // std::cout << "取出任务" << std::endl;
//                 ThreadPoolAssisted task = (pool->Assisted).pop_back();
//                 task.start();
//             }
//         };

//         template<class T, class... Args>
//         bool submit(T* _fun, Args... _args) {
//             //向vector中添加任务
//             //std::cout << "任务添加" << std::endl;
//             Assisted.push_back(ThreadPoolAssisted(_fun, _args...));
//             //判断线程是否等待，如果等待唤醒
//         };
//     };
// }
