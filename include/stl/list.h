#pragma once
namespace stl {


    template <class T> class NodeBase;
    template <class T> class Node;

    template <class T>
    class NodeBase {
    public:
        typedef  NodeBase<T>* BasePtr;
        typedef  Node<T>* NodePtr;

    public:
        BasePtr next;
        NodePtr ptr;

    public:
        NodeBase() : next(nullptr), ptr(nullptr) {};
        NodeBase(BasePtr _next, NodePtr _ptr) : next(_next), ptr(_ptr) {};

    public:
        void operator=(const NodeBase<T>& other) { next = other.next;ptr = other.ptr; };
        void operator++(int) { next = next->next;ptr = next->ptr; };
        bool operator==(const NodeBase<T>& other) { return this == &other; };
        T& operator*() { return ptr->data; };
        T* operator->() { return &(ptr->data); };

    };

    template <class T>
    class Node : public NodeBase<T> {
    public:
        T data;

    public:
        Node() {};
        Node(const T& _data) : data(_data), NodeBase<T>(nullptr, this) {};
    };



    //TODO:list暂时不实现，后续再补充
    template<class T>
    class list {
    public:
        typedef NodeBase<T> iterator;

    public:
        iterator begin;
        iterator end;

    public:
        list();


    };


}