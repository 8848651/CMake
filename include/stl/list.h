#pragma once

namespace stl {

    template <class T> class NodeBase;
    template <class T> class NodeData;

    template <class T>
    class NodeBase {
    public:
        typedef NodeBase<T>* BasePtr;
        typedef NodeData<T>* NodePtr;

    public:
        BasePtr forward;
        BasePtr next;
        NodePtr ptr;

    public:
        NodeBase() : forward(nullptr), next(nullptr), ptr(nullptr) {};
        NodeBase(NodePtr _ptr) : forward(nullptr), next(nullptr), ptr(_ptr) {};
        NodeBase(BasePtr other) : forward(other->forward), next(other->next), ptr(other->ptr) {};

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

    // list class implementation
    template <class T>
    class list {
    public:
        typedef NodeBase<T> iterator;
        typedef NodeBase<T>* pointer;
        typedef NodeData<T> node_data;

    public:
        pointer begin;
        pointer end;
        size_t size;

    public:
        list() : begin(nullptr), end(nullptr), size(0) {}

        // Check if the list is empty
        bool empty() const { return size == 0; }

        // Add a new element to the end
        void push_back(const T& value) {
            pointer new_node = new node_data(value);
            if (empty()) {
                begin = new_node;
                end = new_node;
            }
            else {
                end->next = new_node;
                new_node->forward = end;
                end = new_node;
            }
            ++size;
        }

        // Remove the element from the end
        void pop_back() {
            if (empty()) return;
            pointer temp = end;
            end = end->forward;
            if (end) {
                end->next = nullptr;
            }
            else {
                begin = nullptr;
            }
            delete temp;
            --size;
        }

        // Access the front element
        T& front() {
            if (empty()) throw std::out_of_range("List is empty");
            return begin->ptr->data;
        }

        // Access the back element
        T& back() {
            if (empty()) throw std::out_of_range("List is empty");
            return end->ptr->data;
        }

        // Clear the entire list
        void clear() {
            while (!empty()) {
                pop_back();
            }
        }

        // Destructor to clean up nodes
        ~list() { clear(); };

    };

}
