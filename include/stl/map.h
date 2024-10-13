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
        BasePtr forward;
        BasePtr left;
        BasePtr right;
        NodePtr data_ptr;
        Color color;


     public:
        void operator++(int) {
            //判断当前节点是否有右子树，如果没有将当前节点指向父节点
            if (right == nullptr) {
                //拷贝赋值
                *this = *forward;
                return;
            }
            //如果有右子树,找到右子树的最左节点
            *this = *get_left_most(this);
        };


        //获取能够通过右节点访问到该节点最长父节点
        BasePtr get_parent_right(BasePtr ptr) {
            if (ptr->forward->right == ptr) {
                return get_parent_right(ptr->forward);
            }
            return ptr;
        }

        //获取当前节点的最左子节点
        BasePtr get_left_most(BasePtr ptr) {
            while (ptr->left != nullptr) {
                ptr = ptr->left;
            }
            return ptr;
        }

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
