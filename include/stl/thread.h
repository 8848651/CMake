#pragma once
#include <pthread.h>
namespace stl {


    class thread {
    public:
        pthread_t thread_id;

        template<typename Function, typename... Args>
        thread(Function&& fun, Args&&... args) {
            // 使用 std::bind 来适配参数
            auto task = std::make_shared<std::function<void()>>(
                std::bind(std::forward<Function>(fun), std::forward<Args>(args)...)
            );
            // 创建线程
            pthread_create(&thread_id, nullptr, [](void* arg) -> void* {
                auto task = static_cast<std::shared_ptr<std::function<void()>>>(arg);
                (*task)();  
                return nullptr;
                }, task.get());
        }

        ~thread() {
            pthread_join(thread_id, nullptr);  // 等待线程结束
        }
    };


}