#pragma once

namespace stl {

    template<class T, class U> class BrTreeData;

    enum class Color {
        red,
        black
    };


    template<class T, class U>
    class BrTreeNode {
    public:
        typedef  BrTreeNode<T, U>* BasePtr;
        typedef  BrTreeData<T, U>* DataPtr;

    public:
        BasePtr forward;
        BasePtr left;
        BasePtr right;
        Color color;
        DataPtr data_ptr;

    public:
        BrTreeNode(DataPtr _data_ptr) : forward(nullptr), left(nullptr), right(nullptr), color(Color::red), data_ptr(_data_ptr) {};
        BrTreeNode(BasePtr other) : forward(other->forward), left(other->left), right(other->right), color(other->color), data_ptr(other->data_ptr) {};


    public:
        //中序遍历
        virtual void operator++(int);
        //获取能够通过右节点访问到该节点最长父节点
        BasePtr get_parent_right(BasePtr ptr);
        //获取当前节点的最左子节点
        BasePtr get_left_most(BasePtr ptr);

    public:
        DataPtr operator->() { return data_ptr; };
        bool operator==(const BrTreeNode<T, U>& other) {
            return (data_ptr == other.data_ptr) && (left == other.left) && (right == other.right) && (forward == other.forward);
        };

    };

    template<class T, class U>
    void BrTreeNode<T, U>::operator++(int) {
        //判断当前节点是否有右子树，如果没有将当前节点指向父节点
        if (right == nullptr) {
            //如果是最左子节点，通过右节点访问到该节点最长父节点就是父节点
            //否则就是通过右节点访问到该节点最长父节点的父节点
            BasePtr tmp = get_parent_right(this);
            //拷贝赋值
            *this = *(tmp->forward);
            return;
        }
        //如果有右子树,找到右子树的最左节点
        *this = *get_left_most(right);
    };

    //获取能够通过右节点访问到该节点最长父节点
    template<class T, class U>
    typename BrTreeNode<T, U>::BasePtr BrTreeNode<T, U>::get_parent_right(typename BrTreeNode<T, U>::BasePtr ptr) {
        //迭代器本身地址不能参与比较
        if (ptr->forward->right != nullptr) {
            //能进入这里说明ptr->forward->right != nullptr,并且树一定是有一个最长父节点
            //不存在*(ptr->forward->right) ！= *ptr 这种情况
            if (*(ptr->forward->right) == *ptr) {
                return get_parent_right(ptr->forward);
            }
        }
        return ptr;
    }

    //获取右子树的最左子节点
    template<class T, class U>
    typename BrTreeNode<T, U>::BasePtr  BrTreeNode<T, U>::get_left_most(typename BrTreeNode<T, U>::BasePtr ptr) {
        while (ptr->left != nullptr) {
            ptr = ptr->left;
        }
        return ptr;
    }

    template<class T, class U>
    class BrTreeData :public BrTreeNode<T, U> {
    public:
        T first;
        U second;

    public:
        BrTreeData(T key, U value) : first(key), second(value), BrTreeNode<T, U>(this) {};
    };



    template<class T, class U>
    class BrTree {
        typedef  BrTreeNode<T, U> Iterator;
        typedef  BrTreeNode<T, U>* NodePtr;

    public:
        NodePtr root_ptr = nullptr;
        size_t size = 0;


    public:
        BrTree() : root_ptr(nullptr) {};

    public:
        void insert(T key, U value);
        Iterator GetIterator() { return root_ptr->get_left_most(root_ptr); };
        NodePtr node_insert(NodePtr data_ptr, NodePtr root_ptr);
        void traverse_test();
    };

    template<class T, class U>
    void BrTree<T, U>::insert(T key, U value) {
        BrTreeData<T, U>* data_ptr = new BrTreeData<T, U>(key, value);
        if (root_ptr == nullptr) {
            root_ptr = data_ptr;
            root_ptr->color = Color::black;
            size++;
            return;
        }
        node_insert(data_ptr, root_ptr);
        //if (data_ptr == nullptr) { return; }
        //调整红黑树

        size++;
    };

    //树的插入操作
    template<class T, class U>
    typename BrTree<T, U>::NodePtr BrTree<T, U>::node_insert(typename BrTree<T, U>::NodePtr data_ptr, typename BrTree<T, U>::NodePtr root_ptr) {
        if ((*data_ptr)->first < (*root_ptr)->first) {
            if (root_ptr->left == nullptr) {
                root_ptr->left = data_ptr;
                data_ptr->forward = root_ptr;
                if (root_ptr->color == Color::red) { return data_ptr; }
                return nullptr;
            }
            return node_insert(data_ptr, root_ptr->left);
        }
        else if ((*data_ptr)->first > (*root_ptr)->first) {
            if (root_ptr->right == nullptr) {
                root_ptr->right = data_ptr;
                data_ptr->forward = root_ptr;
                if (root_ptr->color == Color::red) { return data_ptr; }
                return nullptr;
            }
            return node_insert(data_ptr, root_ptr->right);
        }
        (*root_ptr)->second = (*data_ptr)->second;
        return nullptr;

    };


    //中序遍历
    template<class T, class U>
    void BrTree<T, U>::traverse_test() {
        // std::cout << "root first: " << (*(root_ptr))->first << std::endl;
        // std::cout << "root second: " << (*(root_ptr))->second << std::endl;
        // std::cout << "Iterator first: " << GetIterator()->first << std::endl;
        // std::cout << "Iterator second: " << GetIterator()->second << std::endl;
        //获取树的最左子节点
        BrTreeNode<T, U> ptr{ GetIterator() };
        for (int i = 0;i < size - 1;i++) {
            std::cout << "first  " << ptr->first << " second " << ptr->second << " left: "
                << ptr.left << " right: " << ptr.right << " forward: " << ptr.forward << std::endl;
            ptr++;
        }
        std::cout << "first  " << ptr->first << " second " << ptr->second << " left: "
            << ptr.left << " right: " << ptr.right << " forward: " << ptr.forward << std::endl;

    };

}

