#pragma once
namespace stl {



    template<class T, class U>
    class is_same {
    public:
        typedef std::false_type type;
        static type value;
    };

    template<class T>
    class is_same<T, T> {
    public:
        typedef std::true_type type;
        static type value;
    };

    //-------------------------------------------

    template<int T, int U>
    class is_nmber_add {
    public:
        constexpr static int value = T + U;
    };

    template<int T, int U>
    class is_nmber_minus {
    public:
        constexpr static int value = T >= U ? T - U : 0;
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


    //-------------------------------------------


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


    //-------------------------------------------

    // vector<int> a(10);
    template <class T>
    void constructor(T* first, T* last, std::true_type) {
        //第三个为设置比特位数
        memset(first, static_cast<unsigned char>(0), static_cast<size_t>(last - first) * sizeof(T));
    }

    // vector<A> a(10);
    template <class T>
    void constructo(T* first, T* last, std::false_type) {
        while (first < last) {
            auto temp = first;
            ::new ((void*)temp) T();
            first++;
        }
    }

    // vector<int> a(10,10);
    template <class T>
    void constructor(T* first, T* last, const T& value, std::true_type) {
        if (first == last) { *first = value; return; }
        for (T* ptr = first; ptr != last; ++ptr) {
            *ptr = value;
        }
    }

    // vector<A> a(10,{10});
    template <class T>
    void constructor(T* first, T* last, const T& value, std::false_type) {
        if (first == last) {
            ::new ((void*)first) T(value);
            return;
        }
        while (first < last) {
            auto temp = first;
            ::new ((void*)temp) T(value);
            first++;
        }
    }


}