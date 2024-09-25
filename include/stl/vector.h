#pragma once
namespace stl {



    template<typename T>
    class vector {
    public:
        int size;
        T* begin;
        T* end;
        T data[16];

    public:
        vector();
        bool add(T value);
        bool remove(T value);
    };

    template<typename T>
    vector<T>::vector() {
        size = 0;
        begin = data;
        end = data + size;
    }

    template<typename T>
    bool vector<T>::add(T value) {
        if (size == 16) {
            return false;
        }
        *(end++) = value;
        size++;
        return true;
    }

    template<typename T>
    bool vector<T>::remove(T value) {
        for (T* it = begin; it != end; it++) {
            if (*it == value) {
                *it = *(end - 1);
                end--;
                size--;
                return true;
            }
            return false;
        }
    }
}