#pragma once
#include "tool.h"
namespace stl {



    template<typename T, typename U>
    class is_same {
    public:
        typedef std::false_type type;
        static type value;
    };

    template<typename T>
    class is_same<T, T> {
    public:
        typedef std::true_type type;
        static type value;
    };

    //-------------------------------------------

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

    template<typename T>
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


    //-------------------------------------------


    template<bool T>
    class void_type;

    template<>
    class void_type<true> {
    public:
        typedef void type;
    };


    template<typename T, typename U = void>
    class is_same_temp {
    public:
        typedef std::false_type type;
    };

    template<typename T>
    class is_same_temp<T*, void> {
    public:
        typedef std::true_type type;
    };

    template<typename T>
    class is_same_temp<T, typename stl::void_type<stl::is_fundamental_type<T>::value>::type> {
    public:
        typedef std::true_type type;
    };


}