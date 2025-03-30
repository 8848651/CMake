#pragma once
namespace stl {

    // vector<int> a(10);
    template <class T>
    void constructor(T* first, T* last, std::true_type) {
        //第三个为设置比特位数
        memset(first, static_cast<unsigned char>(0), static_cast<size_t>(last - first) * sizeof(T));
    }

    // vector<A> a(10);
    template <class T>
    void constructo(T* first, T* last, std::false_type) {
        while (first < last) {
            auto temp = first;
            ::new ((void*)temp) T();
            first++;
        }
    }

    // vector<int> a(10,10);
    template <class T>
    void constructor(T* first, T* last, const T& value, std::true_type) {
        if (first == last) { *first = value; return; }
        for (T* ptr = first; ptr != last; ++ptr) {
            *ptr = value;
        }
    }

    // vector<A> a(10,{10});
    template <class T>
    void constructor(T* first, T* last, const T& value, std::false_type) {
        if (first == last) {
            ::new ((void*)first) T(value);
            return;
        }
        while (first < last) {
            auto temp = first;
            ::new ((void*)temp) T(value);
            first++;
        }
    }


}