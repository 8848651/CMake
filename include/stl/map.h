#pragma once
#include <functional>
namespace stl {

    enum class Color {
        red,
        black
    };

    template<class key, class value> class NodeBase;
    template<class key, class value> class Node;

    template<class key, class value>
    class NodeBaseMap {
    public:
        typedef  NodeBaseMap<key, value>* BasePtr;
        typedef  NodeMap<key, value>* NodePtr;

    public:
        BasePtr father_ptr;
        BasePtr forward_ptr;
        BasePtr backward_ptr;
        NodePtr data_ptr;
        Color color;

    public:
        NodeBaseMap() : father_ptr(nullptr), forward_ptr(nullptr), backward_ptr(nullptr), data_ptr(nullptr), color(Color::red) {};

    public:
        void operator++(int) {};
        NodePtr operator->() { return data_ptr; };

    };

    template<class key, class value>
    class NodeMap : public NodeBaseMap<key, value> {
    public:
        key first;
        value second;

    public:
        Node() {};
    };


    template<class key, class value>
    class map {
    public:

    };
}
