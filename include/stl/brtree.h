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
        BrTreeNode(const BrTreeNode<T, U>& _node) : forward(_node.forward), left(_node.left), right(_node.right), color(_node.color), data_ptr(nullptr) {};
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
        //节点替换
        static void replace_node(BasePtr old_ptr, BasePtr new_ptr);

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

    //节点替换
    template<class T, class U>
    void BrTreeNode<T, U>::replace_node(typename BrTreeNode<T, U>::BasePtr old_ptr, typename BrTreeNode<T, U>::BasePtr new_ptr) {
        if (old_ptr->forward != nullptr) {
            if (old_ptr->forward->left == old_ptr) {
                old_ptr->forward->left = new_ptr;
            }
            else {
                old_ptr->forward->right = new_ptr;
            }
        }
        new_ptr->forward = old_ptr->forward;
        if (old_ptr->left != nullptr) {
            new_ptr->left = old_ptr->left;
            old_ptr->left->forward = new_ptr;
        }
        if (old_ptr->right != nullptr) {
            new_ptr->right = old_ptr->right;
            old_ptr->right->forward = new_ptr;
        }
        new_ptr->color = old_ptr->color;
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
        bool is_pointer_first = false;
        bool is_pointer_second = false;
        T first;
        U second;

    public:
        BrTreeData(T key, U value) : first(key), second(value), BrTreeNode<T, U>(this) {
            is_pointer_first = std::is_pointer<T>::value;
            is_pointer_second = std::is_pointer<U>::value;
        };
    };



    template<class T, class U>
    class BrTree {
        typedef  BrTreeNode<T, U> Iterator;
        typedef  BrTreeNode<T, U>* NodePtr;

    public:
        NodePtr root_ptr = nullptr;


    public:
        BrTree() : root_ptr(nullptr) {};

    public:
        void insert(T key, U value);
        //获取树的最左子节点,即迭代器的起点
        Iterator get_iterator(NodePtr ptr);
        //插入节点
        NodePtr node_insert(NodePtr data_ptr, NodePtr root_ptr);
        //中序遍历
        void traverse_test();
        //树插入后调整红黑树
        void trim(NodePtr ptr);
        //获取最大值节点
        NodePtr get_max_point(NodePtr ptr);
        NodePtr get_min_point(NodePtr ptr);
        NodePtr get_max_point_1(NodePtr ptr);
        NodePtr get_max_point_2(NodePtr ptr);
        //获取当前节点下所有节点的数量
        size_t node_size(NodePtr ptr);
        //根据key查找节点
        NodePtr find_node(T key);
        //删除节点
        void remove(T key);
        //bst普通删除节点
        void bst_remove(NodePtr ptr);
        //红黑树删除节点
        void br_remove(NodePtr ptr);



    };
    template<class T, class U>
    typename BrTree<T, U>::Iterator BrTree<T, U>::get_iterator(typename BrTree<T, U>::NodePtr ptr) {
        NodePtr temp = ptr->get_left_most(ptr);
        //BrTreeNode<T, U> 有一个BrTreeNode<T, U>*为参数的构造函数
        return static_cast<Iterator>(temp);
    }

    template<class T, class U>
    void BrTree<T, U>::insert(T key, U value) {
        NodePtr data_ptr = new BrTreeData<T, U>(key, value);
        if (root_ptr == nullptr) {
            root_ptr = data_ptr;
            root_ptr->color = Color::black;
            return;
        }
        data_ptr = node_insert(data_ptr, root_ptr);
        if (data_ptr == nullptr) { return; }
        //调整红黑树
        //trim(data_ptr);
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

    //从效果上看最优雅
    template<class T, class U>
    typename BrTree<T, U>::NodePtr BrTree<T, U>::get_max_point(typename BrTree<T, U>::NodePtr ptr) {
        if (ptr == nullptr) { return nullptr; }
        NodePtr right_ptr = get_max_point(ptr->right);
        if (right_ptr == nullptr || (*ptr)->first > (*right_ptr)->first) { return ptr; }
        return right_ptr;
    };

    template<class T, class U>
    typename BrTree<T, U>::NodePtr BrTree<T, U>::get_min_point(typename BrTree<T, U>::NodePtr ptr) {
        if (ptr == nullptr) { return nullptr; }
        NodePtr right_ptr = get_min_point(ptr->right);
        if (right_ptr == nullptr || (*ptr)->first < (*right_ptr)->first) { return ptr; }
        return right_ptr;
    };

    //获取最大值节点,不优雅
    //重构 获取最大值节点只需要获取最右子树即可
    //这个是集遍历查询与一体的
    template<class T, class U>
    typename BrTree<T, U>::NodePtr BrTree<T, U>::get_max_point_1(typename BrTree<T, U>::NodePtr ptr) {
        if (ptr == nullptr) { return nullptr; }
        NodePtr right_ptr = get_max_point_1(ptr->right);
        NodePtr left_ptr = get_max_point_1(ptr->left);
        if (right_ptr == nullptr && left_ptr == nullptr) { return ptr; }
        if (right_ptr == nullptr) {
            //比较左子节点和根节点
            if ((*left_ptr)->first > (*ptr)->first) {
                return left_ptr;
            }
            else {
                return ptr;
            }
        }
        if (left_ptr == nullptr) {
            //比较右子节点和根节点
            if ((*right_ptr)->first > (*ptr)->first) {
                return right_ptr;
            }
            else {
                return ptr;
            }
        }
        //比较左右子节点和根节点
        if (((*ptr)->first > (*left_ptr)->first) && (*ptr)->first > (*right_ptr)->first) {
            return ptr;
        }
        if ((*left_ptr)->first > (*right_ptr)->first) {
            return left_ptr;
        }
        else {
            return right_ptr;
        }
    };

    //获取最大值节点
    template<class T, class U>
    typename BrTree<T, U>::NodePtr BrTree<T, U>::get_max_point_2(typename BrTree<T, U>::NodePtr ptr) {
        BrTreeNode<T, U> max_ptr = ptr;
        BrTreeNode<T, U> iterator_ptr = get_iterator(ptr);
        size_t size_temp = node_size(ptr);
        for (int i = 0;i < size_temp - 1;i++) {
            if (iterator_ptr->first > max_ptr->first) {
                max_ptr = iterator_ptr->data_ptr;
            }
            iterator_ptr++;
        }
        if (iterator_ptr->first > max_ptr->first) {
            max_ptr = iterator_ptr->data_ptr;
        }
        return max_ptr->data_ptr;
    };

    //获取当前节点下所有节点的数量
    template<class T, class U>
    size_t BrTree<T, U>::node_size(typename BrTree<T, U>::NodePtr ptr) {
        if (ptr == nullptr) { return 0; }
        return 1 + node_size(ptr->left) + node_size(ptr->right);
    };

    //根据key查找节点
    template<class T, class U>
    typename BrTree<T, U>::NodePtr BrTree<T, U>::find_node(T key) {
        BrTreeNode<T, U> iterator_ptr = get_iterator(root_ptr);
        size_t size_temp = node_size(root_ptr);
        for (int i = 0;i < size_temp - 1;i++) {
            if (iterator_ptr->first == key) {
                return iterator_ptr->data_ptr;
            }
            iterator_ptr++;
        }
        if (iterator_ptr->first == key) {
            return iterator_ptr->data_ptr;
        }
        return nullptr;
    }

    //删除节点
    template<class T, class U>
    void BrTree<T, U>::remove(T key) {
        NodePtr temp = find_node(key);
        bst_remove(temp);
    }

    //bst普通删除节点,取左子树最大值或右子树最小值替换当前节点
    template<class T, class U>
    void BrTree<T, U>::bst_remove(NodePtr node_ptr) {
        bool is_root = node_ptr == root_ptr;
        if (node_ptr->left != nullptr && node_ptr->right != nullptr) {
            //取左子树最大值或右子树最小值替换当前节点
            NodePtr max_ptr = get_max_point(node_ptr->left);
            std::cout << "max_ptr first  " << (*max_ptr)->first << " second " << (*max_ptr)->second << std::endl;
            NodePtr temp_ptr = new BrTreeNode<T, U>(*max_ptr);
            BrTreeNode<T, U>::replace_node(max_ptr, temp_ptr);
            BrTreeNode<T, U>::replace_node(node_ptr, max_ptr);
            return bst_remove(temp_ptr);
        }
        if (node_ptr->left != nullptr) {
            if (is_root) {
                root_ptr = node_ptr->left;
                root_ptr->forward = nullptr;
            }
            else {
                //注意这里为什么没有用节点替换,相邻节点可能造成this->left=this情况，在遍历时爆栈
                if (node_ptr->forward->left == nullptr) {
                    node_ptr->forward->right = node_ptr->left;
                }
                else {
                    if ((*(node_ptr->forward->left)) == *node_ptr) {
                        node_ptr->forward->left = node_ptr->left;
                    }
                    else {
                        node_ptr->forward->right = node_ptr->left;
                    }
                }
                node_ptr->left->forward = node_ptr->forward;
            }
            return;
        }
        if (node_ptr->right != nullptr) {
            if (is_root) {
                root_ptr = node_ptr->right;
                root_ptr->forward = nullptr;
            }
            else {
                if (node_ptr->forward->left == nullptr) {
                    node_ptr->forward->right = node_ptr->right;
                }
                else {
                    if ((*(node_ptr->forward->left)) == *node_ptr) {
                        node_ptr->forward->left = node_ptr->right;
                    }
                    else {
                        node_ptr->forward->right = node_ptr->right;
                    }
                }
                node_ptr->right->forward = node_ptr->forward;
            }
            return;
        }
        if (is_root) {
            root_ptr = nullptr;
        }
        else {
            if ((*(node_ptr->forward->left)) == *node_ptr) {
                node_ptr->forward->left = nullptr;
            }
            else {
                node_ptr->forward->right = nullptr;
            }
        }
        return;
    }


    //中序遍历
    template<class T, class U>
    void BrTree<T, U>::traverse_test() {
        BrTreeNode<T, U> ptr = get_iterator(root_ptr);
        std::cout << "Iterator frist  " << ptr->first << "   Iterator second  " << ptr->second << std::endl;
        std::cout << "root frist  " << (*root_ptr)->first << "    root second  " << (*root_ptr)->second << std::endl;
        // //获取树的最左子节点
        std::cout << "开始遍历" << std::endl;
        size_t size = node_size(root_ptr);
        for (int i = 0;i < size - 1;i++) {
            std::cout << "first  " << ptr->first << " second " << ptr->second << std::endl;
            //std::cout << " left: " << ptr.left << " right: " << ptr.right << " forward: " << ptr.forward << std::endl;
            ptr++;
        }
        std::cout << "first  " << ptr->first << " second " << ptr->second << std::endl;
        //std::cout << " left: " << ptr.left << " right: " << ptr.right << " forward: " << ptr.forward << std::endl;

    };

}

