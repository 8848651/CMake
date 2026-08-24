#pragma once
#include <mutex>
#include "construct.h"
#include "too.h"
#include "template.h"
namespace stl {

    template<class T>
    class vector {
    public:
        typedef typename stl::is_same_temp<T>::type type;
        typedef T* iterator;
        typedef T reference;

    public:
        //std::mutex mtx;
        size_t* ReferenceCount = new size_t(0);
        iterator begin;
        iterator cap;
        iterator end;

    public:
        vector();
        vector(size_t size);
        vector(size_t size, T&& value);
        vector(const vector<T>& other);
        reference operator[](size_t index) { return *(begin + index); };
        ~vector();


    public:
        void push_back(const T& value);
        reference pop_back();
        size_t size() const { return cap - begin; };
        size_t length() const { return end - begin; };

    };

    template<class T>
    stl::vector<T>::vector() {
        begin = cap = static_cast<T*>(malloc(16 * sizeof(T)));
        end = begin + 16;
        (*ReferenceCount)++;
    }

    template<class T>
    stl::vector<T>::vector(size_t size) {
        size_t length = size < 16 ? 16 : size;
        begin = static_cast<T*>(malloc(length * sizeof(T)));
        cap = begin + size;
        end = begin + length;
        constructor(begin, cap, type());
        (*ReferenceCount)++;
    }

    template<class T>
    stl::vector<T>::vector(size_t size, T&& value) {
        size_t length = size < 16 ? 16 : size;
        begin = static_cast<T*>(malloc(length * sizeof(T)));
        cap = begin + size;
        end = begin + length;
        constructor(begin, cap, value, type());
        (*ReferenceCount)++;
    }

    template<class T>
    stl::vector<T>::vector(const vector<T>& other) {
        begin = other.begin;
        cap = other.cap;
        end = other.end;
        ReferenceCount = other.ReferenceCount;
        (*ReferenceCount)++;
    }

    template<class T>
    typename stl::vector<T>::reference stl::vector<T>::pop_back() {
        //std::lock_guard<std::mutex> lock(mtx);
        return *(--cap);
    }

    template<class T>
    void stl::vector<T>::push_back(const T& value) {
        //std::lock_guard<std::mutex> lock(mtx);
        size_t length = cap - begin;
        if (cap == end) {
            //进行扩容
            size_t new_length = length * 2;
            T* new_begin = static_cast<T*>(malloc(new_length * sizeof(T)));
            stl::my_memcpy(begin, new_begin, length * sizeof(T));
            if (ReferenceCount == 0) { free(begin); }
            begin = new_begin;
            cap = begin + length;
            end = begin + new_length;
        }
        constructor(cap, cap + 1, value, type());
        cap++;
    }

    template<class T>
    stl::vector<T>::~vector() {
        if (ReferenceCount == 0) {
            free(begin);
        }
        ReferenceCount--;
    }

}