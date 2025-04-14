#pragma once
#include "bind.h"
namespace stl {

    template<bool B, class T>
    class function_execute_ptr;

    //lambda执行类
    template<int... Is>
    class function_execute_ptr<true, IntList<Is...>> {
    public:
        //有参
        template<class T, class M, class... Args>
        static void* execute(M& lambda_ptr, Tuple<Args...>& args) {
            constexpr bool is_same_1 = stl::is_same<T, decltype(lambda_ptr(stl::TupleFindElement<Is>::find(args.base)...))>::value;
            static_assert(is_same_1, "返回值类型不匹配");
            lambda_ptr(stl::TupleFindElement<Is>::find(args.base)...);
            return nullptr;
        };

        //无参
        template<class T, class M>
        static void* execute(M& lambda_ptr) {
            constexpr bool is_same_1 = stl::is_same<T, decltype(lambda_ptr())>::value;
            static_assert(is_same_1, "返回值类型不匹配");
            lambda_ptr();
            return nullptr;
        };
    };

    //有返回值
    template<int... Is>
    class function_execute_ptr<false, IntList<Is...>> {
    public:
        template<class T, class M, class... Args>
        static void* execute(M& lambda_ptr, Tuple<Args...>& args) {
            constexpr bool is_same_1 = stl::is_same<T, decltype(lambda_ptr(stl::TupleFindElement<Is>::find(args.base)...))>::value;
            static_assert(is_same_1, "返回值类型不匹配");
            return stl::is_type<T>::execute(lambda_ptr(stl::TupleFindElement<Is>::find(args.base)...));
        };

        template<class T, class M>
        static void* execute(M& lambda_ptr) {
            constexpr bool is_same_1 = stl::is_same<T, decltype(lambda_ptr())>::value;
            static_assert(is_same_1, "返回值类型不匹配");
            return stl::is_type<T>::execute(lambda_ptr());
        };
    };


    //lambda包装类
    template<class M>
    class function_wrapper_ptr {
    public:
        M _ptr;

        function_wrapper_ptr(const M& _ptr) : _ptr(_ptr) {};

        template<class T, class... U>
        static void* call_1(void* obj, void* args) {
            function_wrapper_ptr<M>* data = static_cast<function_wrapper_ptr<M>*>(obj);
            Tuple<U...>* _args = static_cast<Tuple<U...>*>(args);
            static constexpr int _size = sizeof...(U) - 1;
            constexpr bool is_same_2 = stl::is_same<T, void>::value;
            return function_execute_ptr<is_same_2, typename AssistedQueue<_size>::QueueData>::template execute<T, M, U...>(data->_ptr, *_args);
        };

        template<class T>
        static void* call_2(void* obj, void* args) {
            function_wrapper_ptr<M>* data = static_cast<function_wrapper_ptr<M>*>(obj);
            constexpr bool is_same_2 = stl::is_same<T, void>::value;
            return function_execute_ptr<is_same_2, typename AssistedQueue<7>::QueueData>::template execute<T, M>(data->_ptr);
        };

        ~function_wrapper_ptr() {
            stl::br_destroy(_ptr);
        };

    };

    template<bool B, class T, class... U>
    class function_base_ptr {
    public:
        template<class M>
        static auto execute() {
            return function_wrapper_ptr<M>::template call_1<T, U...>;
        }
    };

    template<class T, class... U>
    class function_base_ptr<false, T, U...> {
    public:
        template<class M>
        static auto execute() {
            return function_wrapper_ptr<M>::template call_2<T>;
        }
    };

    template<class T>
    class function_base;

    template<class T, class... U>
    class function_base<T(U...)> {
    public:
        static constexpr int _size = sizeof...(U);
    public:
        void* _data = nullptr;
        void* (*fun)(void*, void*) = nullptr;

        function_base() : _data(nullptr), fun(nullptr) {};
        //自定义stl::bind
        template<class... M>
        function_base(const stl::anybind<M...>& bind) {
            this->_data = new function_wrapper_ptr<stl::anybind<M...>>(bind);
            this->fun = stl::function_base_ptr<_size, T, U...>::template execute<stl::anybind<M...>>();
        };
        //自定义stl::bind_class
        template<class... M>
        function_base(const stl::anybind_class<M...>& bind) {
            this->_data = new function_wrapper_ptr<stl::anybind_class<M...>>(bind);
            this->fun = stl::function_base_ptr<_size, T, U...>::template execute<stl::anybind_class<M...>>();
        };
        //lambda表达式,std::bind
        template<class M>
        function_base(const M& lambda) {
            this->_data = new function_wrapper_ptr<M>(lambda);
            this->fun = stl::function_base_ptr<_size, T, U...>::template execute<M>();
        };
        //函数指针
        template<class M>
        function_base(M* ptr) {
            this->_data = new function_wrapper_ptr<M*>(ptr);
            this->fun = stl::function_base_ptr<_size, T, U...>::template execute<M*>();
        };

        template<class... M>
        T operator()(M... args) {
            Tuple<M...>* _args = new Tuple<M...>(args...);
            void* _temp = fun(_data, _args);
            delete _args;
            return is_type<T>::_cast(_temp);
        };

        T operator()() {
            void* _temp = fun(_data, nullptr);
            return is_type<T>::_cast(_temp);
        };

        ~function_base() {
            if (_data != nullptr) {
                delete _data;
            }
        }

    };

    //-----------------------------------------------------------------------------------------
    //function

    template<class T>
    class function;

    template<class T, class... U>
    class function<T(U...)> {
    public:
        int* _number = new int(1);
        function_base<T(U...)>* _fun = nullptr;

        function() : _fun(nullptr) {};
        function(const function& other) : _fun(other._fun), _number(other._number) { *_number += 1; };
        template<class... M>
        function(const stl::anybind<M...>& bind) :_fun(new function_base<T(U...)>(bind)) {};
        template<class... M>
        function(const stl::anybind_class<M...>& bind) :_fun(new function_base<T(U...)>(bind)) {};
        template<class M>
        function(const M& lambda_ptr) : _fun(new function_base<T(U...)>(lambda_ptr)) {};
        template<class M>
        function(M* lambda_ptr) : _fun(new function_base<T(U...)>(lambda_ptr)) {};


        T operator()(U... args) {
            return (*_fun)(args...);
        };

        ~function() {
            if (*_number == 1) {
                delete _fun;
            }
            *_number -= 1;
        }
    };


}