#pragma once
#include <mutex>
#include <stl/construct.h>
#include <stl/tuple.h>
#include <stl/too.h>
namespace stl {

    //TODO:动态队列
    template <class T>
    class queue_t;


    //TODO:静态队列
    template <class T>
    class queue {
    public:
        typedef typename stl::is_same_temp<T>::type type;

    private:
        static const int SIZE = 5;
        size_t length = 0;
        std::mutex mtx;
        T* _begin;
        T* begin;
        T* end;

    public:
        queue();
        ~queue() { free(_begin); };
        void push(const T& value);
        T pop();
        bool empty() const { return begin == end; };
        size_t size() { return length; };
    };

    template <class T>
    queue<T>::queue() {
        _begin = static_cast<T*>(malloc(sizeof(T) * SIZE));
        end = begin = _begin;

    };

    template <class T>
    void queue<T>::push(const T& value) {
        std::lock_guard<std::mutex> lock(mtx);
        auto temp = end + 1;

        if ((temp == begin) || (end == _begin + SIZE - 1 && length == SIZE - 1)) {
            //丢弃当前任务
            throw std::overflow_error("队列已满,无法加入新任务");
        }

        constructor(end, end, value, type());

        if (end == _begin + SIZE) {
            end = _begin;
        }
        else {
            end++;
        }
        length++;

    };

    template <class T>
    T queue<T>::pop() {
        std::lock_guard<std::mutex> lock(mtx);
        
        if (begin == end) {
            throw std::overflow_error("队列为空,无法取出任务");
        }
        T* temp = begin;

        if (begin == _begin + SIZE) {
            begin = _begin;
        }
        else {
            begin++;
        }
        length--;

        return *temp;

    }



}