#pragma once
namespace stl {


    template<typename T>
    class node {
    public:
        T* data;
        node<T>* next;
    public:
        node() : data(nullptr), next(nullptr) {};
        node(T data) : data(new T(data)), next(nullptr) {};
        node(const node<T>& other) : data(new T(*other.data)), next(other.next) {};
        void operator=(const node<T>& other) { data = other.data; next = other.next; };
        void operator++(int) { data = next->data; next = next->next; };
        bool operator==(const node<T>& other) { return (data == other.data) && (next == other.next); };
        T& operator*() { return *data; };
        T* operator->() { return data; };
        ~node() { delete data; };
    };


    //TODO:list暂时不实现，后续再补充
    template<typename T>
    class list {
    public:
        typedef node<T> iterator;

    public:
        iterator begin;
        iterator end;

    public:
        list() : begin(nullptr), end(nullptr) {};


    };


}