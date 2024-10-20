#pragma once
namespace stl {


    template <class T> class NodeBase;
    template <class T> class NodeData;

    template <class T>
    class NodeBase {
    public:
        typedef  NodeBase<T>* BasePtr;
        typedef  NodeData<T>* NodePtr;

    public:
        BasePtr forward;
        BasePtr next;
        NodePtr ptr;

    public:
        NodeBase() : forward(nullptr), next(nullptr), ptr(nullptr) {};
        NodeBase(NodePtr _ptr) : forward(nullptr), next(nullptr), ptr(_ptr) {};
        NodeBase(BasePtr other) :forward(other->forward), next(other->next), ptr(other->ptr) {};

    public:
        void operator++(int) { *this = *next; };
        bool operator==(const NodeBase<T>& other) { return forward == other.forward && next == other.next && ptr == other.ptr; };
        T& operator*() { return ptr->data; };
        T* operator->() { return &(ptr->data); };

    };

    template <class T>
    class NodeData : public NodeBase<T> {
    public:
        T data;

    public:
        NodeData() {};
        NodeData(const T& _data) : data(_data), NodeBase<T>(this) {};
    };



    //TODO:list暂时不实现，后续再补充
    template<class T>
    class list {
    public:
        typedef NodeBase<T> iterator;
        typedef NodeBase<T>* pointer;

    public:
        pointer begin;
        pointer end;

    public:
        list();


    };


}