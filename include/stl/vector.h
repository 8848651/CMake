#pragma once
namespace stl {



    template<typename T>
    class vector {
    public:
        typedef T* iterator;

    public:
        iterator begin;
        iterator cap;
        iterator end;

    public:
        vector();
        vector(size_t size);
        vector(size_t size, const T& value);


    public:
        void push_back(const T& value);
        void pop_back();
        size_t size() const { return cap - begin; };

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
    }

    template<typename T>
    void vector<T>::push_back(const T& value) {

    }


}