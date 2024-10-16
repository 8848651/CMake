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
        void operator++(int);
        //获取能够通过右节点访问到该节点最长父节点
        BasePtr get_parent_right(BasePtr ptr);
        //获取当前节点的最左子节点
        BasePtr get_left_most(BasePtr ptr);

    public:
        //左旋
        static void rotate_left(BasePtr ptr);
        //右旋
        static void rotate_right(BasePtr ptr);

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
            if (*(ptr->forward->right) == *ptr) {
                return get_parent_right(ptr->forward);
            }
            //不会执行,这是判断的另一种方法通过迭代器中data_ptr 即原节点指针
            if (ptr->forward->right == ptr->data_ptr) {
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

    //左旋
    template<class T, class U>
    void BrTreeNode<T, U>::rotate_left(typename BrTreeNode<T, U>::BasePtr ptr) {
        BasePtr futher_ptr = ptr->forward;
        if (futher_ptr->forward != nullptr) {
            if (futher_ptr->forward->left == futher_ptr) {
                futher_ptr->forward->left = ptr;
            }
            else {
                futher_ptr->forward->right = ptr;
            }
        }
        if (ptr->right != nullptr) {
            ptr->right->forward = futher_ptr;
        }
        ptr->forward = futher_ptr->forward;
        futher_ptr->forward = ptr;
        futher_ptr->left = ptr->right;
        ptr->right = futher_ptr;
    }

    //右旋
    template<class T, class U>
    void BrTreeNode<T, U>::rotate_right(typename BrTreeNode<T, U>::BasePtr ptr) {
        BasePtr futher_ptr = ptr->forward;
        if (futher_ptr->forward != nullptr) {
            if (futher_ptr->forward->left == futher_ptr) {
                futher_ptr->forward->left = ptr;
            }
            else {
                futher_ptr->forward->right = ptr;
            }
        }
        if (ptr->left != nullptr) {
            ptr->left->forward = futher_ptr;
        }
        ptr->forward = futher_ptr->forward;
        futher_ptr->forward = ptr;
        futher_ptr->right = ptr->left;
        ptr->left = futher_ptr;
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
        Iterator test() {
            NodePtr temp = nullptr;
            return static_cast<Iterator>(temp);
        };

    public:
        NodePtr root_ptr = nullptr;
        size_t size = 0;


    public:
        BrTree() : root_ptr(nullptr) {};

    public:
        void insert(T key, U value);
        //获取树的最左子节点
        Iterator get_iterator() {
            NodePtr temp = root_ptr->get_left_most(root_ptr);
            //BrTreeNode<T, U> 有一个BrTreeNode<T, U>*为参数的构造函数
            return static_cast<Iterator>(temp);
        };
        //插入节点
        NodePtr node_insert(NodePtr data_ptr, NodePtr root_ptr);
        //中序遍历
        void traverse_test();
        //树插入后调整红黑树
        void trim(NodePtr ptr);
        //获取最大值节点
        NodePtr get_max_point_1(NodePtr ptr);
        NodePtr get_max_point_2(NodePtr ptr);

    };

    template<class T, class U>
    void BrTree<T, U>::insert(T key, U value) {
        size++;
        NodePtr data_ptr = new BrTreeData<T, U>(key, value);
        if (root_ptr == nullptr) {
            root_ptr = data_ptr;
            root_ptr->color = Color::black;
            return;
        }
        data_ptr = node_insert(data_ptr, root_ptr);
        if (data_ptr == nullptr) { return; }
        //调整红黑树
        trim(data_ptr);
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


    template<class T, class U>
    void BrTree<T, U>::trim(typename BrTree<T, U>::NodePtr ptr) {
        if (ptr == root_ptr || ptr->forward == root_ptr) { root_ptr->color = Color::black; return; }
        NodePtr uncle_ptr = nullptr;
        NodePtr futher_ptr = ptr->forward;
        NodePtr grand_futher_ptr = futher_ptr->forward;
        bool is_left_child = ptr == ptr->forward->left;
        bool is_left_futhe = futher_ptr == grand_futher_ptr->left;
        if (is_left_futhe) {
            uncle_ptr = grand_futher_ptr->right;
        }
        else {
            uncle_ptr = grand_futher_ptr->left;
        }
        //如果叔叔节点是红色,则将父节点和叔叔节点染成黑色,并将爷爷节点作为插入节点
        if (uncle_ptr != nullptr && uncle_ptr->color == Color::red) {
            futher_ptr->color = Color::black;
            uncle_ptr->color = Color::black;
            grand_futher_ptr->color = Color::red;
            return trim(grand_futher_ptr);
        }

        if (is_left_futhe) {
            if (is_left_child) {
                // LL型
                futher_ptr->color = Color::black;
                grand_futher_ptr->color = Color::red;
                if (grand_futher_ptr == root_ptr) {
                    root_ptr = futher_ptr;
                }
                BrTreeNode<T, U>::rotate_left(futher_ptr);
                return;
            }
            else {
                // LR型
                ptr->color = Color::black;
                grand_futher_ptr->color = Color::red;
                if (grand_futher_ptr == root_ptr) {
                    root_ptr = ptr;
                }
                BrTreeNode<T, U>::rotate_right(ptr);
                BrTreeNode<T, U>::rotate_left(ptr);
                return;
            }
        }
        else {
            if (is_left_child) {
                // RL型
                ptr->color = Color::black;
                grand_futher_ptr->color = Color::red;
                if (grand_futher_ptr == root_ptr) {
                    root_ptr = ptr;
                }
                BrTreeNode<T, U>::rotate_left(ptr);
                BrTreeNode<T, U>::rotate_right(ptr);
                return;
            }
            else {
                // RR型
                futher_ptr->color = Color::black;
                grand_futher_ptr->color = Color::red;
                if (grand_futher_ptr == root_ptr) {
                    root_ptr = futher_ptr;
                }
                BrTreeNode<T, U>::rotate_right(futher_ptr);
                return;
            }
        }

    }

    //获取最大值节点
    template<class T, class U>
    typename BrTree<T, U>::NodePtr BrTree<T, U>::get_max_point_1(typename BrTree<T, U>::NodePtr ptr) {
        if (ptr == nullptr) { return nullptr; }
        NodePtr right_ptr = get_max_point_1(ptr->right);
        NodePtr left_ptr = get_max_point_1(ptr->left);
        if (right_ptr == nullptr && left_ptr == nullptr) { return ptr; }
        if (right_ptr == nullptr) {
            //比较左子节点和根节点
            if ((*left_ptr)->first >= (*ptr)->first) {
                return left_ptr;
            }
            else {
                return ptr;
            }
        }
        if (left_ptr == nullptr) {
            //比较右子节点和根节点
            if ((*right_ptr)->first >= (*ptr)->first) {
                return right_ptr;
            }
            else {
                return ptr;
            }
        }
        //比较左右子节点和根节点
        if (((*ptr)->first >= (*left_ptr)->first) && (*ptr)->first >= (*right_ptr)->first) {
            return ptr;
        }
        if ((*left_ptr)->first >= (*right_ptr)->first) {
            return left_ptr;
        }
        else {
            return right_ptr;
        }
    };

    //获取最大值节点
    template<class T, class U>
    typename BrTree<T, U>::NodePtr BrTree<T, U>::get_max_point_2(typename BrTree<T, U>::NodePtr ptr) {
        NodePtr max_ptr = ptr;
        BrTreeNode<T, U> iterator_ptr=get_iterator();

    };


    //中序遍历
    template<class T, class U>
    void BrTree<T, U>::traverse_test() {
        std::cout << "Iterator frist  " << get_iterator()->first << "   Iterator second  " << get_iterator()->second << std::endl;
        std::cout << "root frist  " << (*root_ptr)->first << "    root second  " << (*root_ptr)->second << std::endl;
        // //获取树的最左子节点
        std::cout << "开始遍历" << std::endl;
        BrTreeNode<T, U> ptr = get_iterator();
        for (int i = 0;i < size - 1;i++) {
            std::cout << "first  " << ptr->first << " second " << ptr->second << std::endl;
            //std::cout << " left: " << ptr.left << " right: " << ptr.right << " forward: " << ptr.forward << std::endl;
            ptr++;
        }
        std::cout << "first  " << ptr->first << " second " << ptr->second << std::endl;
        //std::cout << " left: " << ptr.left << " right: " << ptr.right << " forward: " << ptr.forward << std::endl;

    };

}

