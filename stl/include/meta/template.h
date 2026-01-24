#pragma once
#include <type_traits>
#include <cstddef>


namespace stl {
    // remove_reference用于去除引用
    // conditional_selector_fun 用于类型判断并返回判断的值
    // enable_if 用于类型判断
    // br_is_pointer 用于判断是否是指针类型
    // br_destroy 用于销毁指针
    // is_same 用于判断两个类型是否相同
    // is_nmber_add 用于两个数字相加
    // is_nmber_minus 用于两个数字相减
    // is_fundamental_type 用于判断是否是基本类型
    // void_type 用于判断是否是void类型
    // is_same_temp 用于判断是否是基本类型和指针类型


    //去除引用
    template<typename T>
    struct remove_reference {
        using type = T;
    };

    template<typename T>
    struct remove_reference<T&> {
        using type = T;
    };

    template<typename T>
    struct remove_reference<T&&> {
        using type = T;
    };

    //移除const
    template<typename T>
    struct remove_const {
        using type = T;
    };

    template<typename T>
    struct remove_const<const T> {
        using type = T;
    };

    template<typename T>
    struct remove_const<const T&> {
        using type = T;
    };

    template<typename T>
    struct remove_const<const T&&> {
        using type = T;
    };

    //添加引用,仅限于bind使用，有很大局限性
    template<typename T>
    struct bindadd_reference {
        using type = const T&;
    };

    template<typename T>
    struct bindadd_reference<const T> {
        using type = const T&;
    };

    template<typename T>
    struct bindadd_reference<const T&> {
        using type = const T&;
    };

    template<typename T>
    struct bindadd_reference<const T&&> {
        using type = const T&&;
    };

    template<typename T>
    struct bindadd_reference<T&> {
        using type = T&;
    };

    template<typename T>
    struct bindadd_reference<T&&> {
        using type = T&&;
    };

    //判断类型是否为const左值引用
    template<typename T>
    struct is_left_const {
        constexpr static bool value = false;
    };

    template<typename T>
    struct is_left_const<const T&> {
        constexpr static bool value = true;
    };

    // 条件选择器模板类
    template <bool Flag, typename T, typename U>
    struct conditional_selector;

    template <typename T, typename U>
    struct conditional_selector<true, T, U> {
        using type = T;
        static T execute(T t, U u) { return std::forward<T>(t); }
    };

    template <typename T, typename U>
    struct conditional_selector<false, T, U> {
        using type = U;
        static U execute(T t, U u) { return std::forward<U>(u); }
    };




    // 条件选择器函数模板
    template <bool Flag, typename T = void, typename U = void>
    struct enable_if {};

    template <typename T, typename U>
    struct enable_if<true, T, U> {
    public:
        using type = T;
    };



    // 指针判断模板类
    template<class T>
    class br_is_pointer {
    public:
        constexpr static bool value = false;
    };

    template<class T>
    class br_is_pointer<T*> {
    public:
        constexpr static bool value = true;
    };


    //通过返回值类型判断是否是指针
    template<class T>
    typename enable_if<stl::br_is_pointer<T>::value>::type br_destroy(T ptr) { delete ptr; }

    template<class T>
    typename enable_if<!stl::br_is_pointer<T>::value>::type br_destroy(T) {}


    //判断两个类型是否相同
    template<class T, class U>
    class is_same {
    public:
        using  type = std::false_type;
        static type value;
    };

    template<class T>
    class is_same<T, T> {
    public:
        using type = std::true_type;
        static type value;
    };

    //两个类型数字相加减，在编译时常量无法直接相加需要变成类型相加后再变回去
    template<size_t T, size_t U>
    class is_nmber_add {
    public:
        constexpr static size_t value = T + U;
    };

    template<size_t T, size_t U>
    class is_nmber_minus {
    public:
        constexpr static size_t value = T >= U ? T - U : 0;
    };

    //判断一个类型是否是基本类型

    template<bool T>
    class value_type {
    public:
        typedef std::false_type type;
    };

    template<>
    class value_type<true> {
    public:
        typedef std::true_type type;
    };

    template<class T>
    class is_fundamental_type {
    public:
        typedef typename stl::value_type<
            stl::is_same<T, int>::value
            || stl::is_same<T, short>::value
            || stl::is_same<T, long>::value
            || stl::is_same<T, long long>::value
            || stl::is_same<T, float>::value
            || stl::is_same<T, double>::value
            || stl::is_same<T, long double>::value
            || stl::is_same<T, char>::value
        >::type type;
        static type value;
    };

    //是否为void类型
    template<class T>
    class is_void_type {
    public:
        typedef typename stl::value_type<stl::is_same<T, void>::value>::type type;
        static type value;
    };

    //判断T是引用还是指针
    template<class T>
    class is_type {
    public:
        static void* execute(const T& u) { return new T{ u }; };
        static auto _cast(void*& u) { return *static_cast<T*>(u); };
    };

    template<>
    class is_type<void> {
    public:
        static void execute(void* u) { return; };
        static void _cast(void*& u) { return; };
    };

    template<class T>
    class is_type<T*> {
    public:
        static void* execute(T*& u) { return u; };;
        static auto _cast(void*& u) { return static_cast<T*>(u); };
    };


    //基本数据类型和指针类型-其他引用类型  常用于初始化
    template<bool T>
    class void_type;

    template<>
    class void_type<true> {
    public:
        typedef void type;
    };


    template<class T, typename U = void>
    class is_same_temp {
    public:
        typedef std::false_type type;
    };

    template<class T>
    class is_same_temp<T*, void> {
    public:
        typedef std::true_type type;
    };

    template<class T>
    class is_same_temp<T, typename stl::void_type<stl::is_fundamental_type<T>::value>::type> {
    public:
        typedef std::true_type type;
    };

    //取参数包第一个值
    template<typename... T>
    struct first_type;

    template<typename T, typename... U>
    struct first_type<T, U...> {
        using type = T;
    };

    template<>
    struct first_type<> {
        using type = void;
    };



}
