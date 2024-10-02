#pragma once
#include <stl/construct.h>
#include <stl/tuple.h>
#include <stl/too.h>
namespace stl {



    template<typename T>
    class vector {
    public:
        typedef typename stl::is_same_temp<T>::type type;
        typedef T* iterator;
        typedef T reference;

    public:
        iterator begin;
        iterator cap;
        iterator end;

    public:
        vector();
        vector(size_t size);
        vector(size_t size, T&& value);
        reference operator[](size_t index) { return *(begin + index); };


    public:
        void push_back(const T& value);
        reference pop_back() { return *(--cap); };
        size_t size() const { return cap - begin; };
        size_t length() const { return end - begin; };

    };

    template<typename T>
    vector<T>::vector() {
        begin = cap = static_cast<T*>(malloc(16 * sizeof(T)));
        end = begin + 16;
    }

    template<typename T>
    vector<T>::vector(size_t size) {
        size_t length = size < 16 ? 16 : size;
        begin = static_cast<T*>(malloc(length * sizeof(T)));
        cap = begin + size;
        end = begin + length;
        constructor_vector(begin, cap, type());
    }

    template<typename T>
    vector<T>::vector(size_t size, T&& value) {
        size_t length = size < 16 ? 16 : size;
        begin = static_cast<T*>(malloc(length * sizeof(T)));
        cap = begin + size;
        end = begin + length;
        constructor_vector(begin, cap, value, type());
    }

    template<typename T>
    void vector<T>::push_back(const T& value) {
        size_t length = cap - begin;
        if (cap == end) {
            //进行扩容
            size_t new_length = length * 2;
            T* new_begin = static_cast<T*>(malloc(new_length * sizeof(T)));
            stl::my_memcpy(begin, new_begin, length * sizeof(T));
            free(begin);
            begin = new_begin;
            cap = begin + length;
            end = begin + new_length;
        }
        constructor_vector(cap, cap + 1, value, type());
        cap++;
    }

}