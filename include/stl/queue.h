#pragma once
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

    public:
        static const int SIZE = 10;
        T* _begin;
        T* begin;
        T* end;

    public:
        queue();
        ~queue() { free(_begin); };
        void push(const T& value);
        T pop();
        bool empty() const { return begin == end; };
    };

    template <class T>
    queue<T>::queue() {
        _begin = static_cast<T*>(malloc(sizeof(T) * SIZE));
        end = begin = _begin;

    };

    template <class T>
    void queue<T>::push(const T& value) {

        if ((end++ == begin) || (end == _begin + SIZE && begin == _begin)) {
            throw std::overflow_error("队列已满");
        }


        if (end == _begin + SIZE) {
            end = _begin;
        }
        else {
            end++;
        }
    };

    template <class T>
    T queue<T>::pop() {
        if (begin == end) { throw std::overflow_error("队列为空"); }
    }



}