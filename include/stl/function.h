#pragma once
#include "bind.h"
namespace stl {


    //bind和lambda一样，都是对包装并在包装选择执行

    template<class T>
    class callbase_function;

    template<bool T>
    class callbase_function_;

    //anybind执行类
    template<>
    class callbase_function_<true> {
    public:
        template<class... M, class... N>
        static void* execute(callbase_function<stl::anybind<M...>>* _base_function, Tuple<N...>* _args) {
            _base_function->_bind((*_args));
            return nullptr;
        }

        template<class... M>
        static void* execute(callbase_function<stl::anybind<M...>>* _base_function) {
            _base_function->_bind();
            return nullptr;
        }
    };

    template<>
    class callbase_function_<false> {
    public:
        template<class... M, class... N>
        static void* execute(callbase_function<stl::anybind<M...>>* _base_function, Tuple<N...>* _args) {
            auto _temp = _base_function->_bind((*_args));
            return is_type<decltype(_temp)>::execute(_temp);
        }

        template<class... M>
        static void* execute(callbase_function<stl::anybind<M...>>* _base_function) {
            auto _temp = _base_function->_bind();
            return is_type<decltype(_temp)>::execute(_temp);
        }
    };


    //anybind包装类
    template<class... T>
    class callbase_function<stl::anybind<T...>> {
    public:
        stl::anybind<T...> _bind;
    public:
        callbase_function(const stl::anybind<T...>& bind) : _bind(bind) {};

        template<class M, class... N>
        static void* call_1(void* obj, void* args) {
            Tuple<N...>* _args = static_cast<Tuple<N...>*>(args);
            callbase_function<stl::anybind<T...>>* _base_function = static_cast<callbase_function<stl::anybind<T...>>*>(obj);
            constexpr bool is_same_1 = stl::is_same<M, decltype(_base_function->_bind((*_args)))>::value;
            static_assert(is_same_1, "返回值类型不匹配");
            constexpr bool is_same_2 = stl::is_same<M, void>::value;
            return callbase_function_<is_same_2>::execute(_base_function, _args);
        };

        template<class M>
        static void* call_2(void* obj, void* args) {
            callbase_function<stl::anybind<T...>>* _base_function = static_cast<callbase_function<stl::anybind<T...>>*>(obj);
            constexpr bool is_same_1 = stl::is_same<M, decltype(_base_function->_bind())>::value;
            static_assert(is_same_1, "返回值类型不匹配");
            constexpr bool is_same_2 = stl::is_same<M, void>::value;
            return callbase_function_<is_same_2>::execute(_base_function);
        };
    };

    template<bool B, class T, class... U>
    class function_fun {
    public:
        template<class... M>
        static auto execute() {
            return callbase_function<stl::anybind<M...>>::template call_1<T, U...>;
        }
    };

    template<class T>
    class function_fun<false, T> {
    public:
        template<class... M>
        static auto execute() {
            return callbase_function<stl::anybind<M...>>::template call_2<T>;
        }
    };

    template<class T>
    class function_bind;

    //function_bind
    template<class T, class... U>
    class function_bind<T(U...)> {
    public:
        static constexpr int _size = sizeof...(U);
    public:
        void* _data = nullptr;
        void* (*fun)(void*, void*) = nullptr;


        function_bind() : _data(nullptr), fun(nullptr) {};
        template<class... M>
        function_bind(const stl::anybind<M...>& bind) {
            this->_data = new callbase_function<stl::anybind<M...>>(bind);
            this->fun = stl::function_fun<_size, T, U...>::template execute<M...>();
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

    };



    //-----------------------------------------------------------------------------------------
    //function_lambda_ptr
    template<bool B, class T>
    class function_lambda_ptr_helper;

    //lambda执行类
    template<int... Is>
    class function_lambda_ptr_helper<true, IntList<Is...>> {
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
    class function_lambda_ptr_helper<false, IntList<Is...>> {
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
    class function_lambda_ptr {
    public:
        M _lambda_ptr;

        function_lambda_ptr(const M& _lambda_ptr) : _lambda_ptr(_lambda_ptr) {};

        template<class T, class... U>
        static void* call_1(void* obj, void* args) {
            function_lambda_ptr<M>* data = static_cast<function_lambda_ptr<M>*>(obj);
            Tuple<U...>* _args = static_cast<Tuple<U...>*>(args);
            static constexpr int _size = sizeof...(U) - 1;
            constexpr bool is_same_2 = stl::is_same<T, void>::value;
            return function_lambda_ptr_helper<is_same_2, typename AssistedQueue<_size>::QueueData>::template execute<T, M, U...>(data->_lambda_ptr, *_args);
        };

        template<class T>
        static void* call_2(void* obj, void* args) {
            function_lambda_ptr<M>* data = static_cast<function_lambda_ptr<M>*>(obj);
            constexpr bool is_same_2 = stl::is_same<T, void>::value;
            return function_lambda_ptr_helper<is_same_2, typename AssistedQueue<7>::QueueData>::template execute<T, M>(data->_lambda_ptr);
        };

    };

    template<bool B, class T, class... U>
    class function_lam {
    public:
        template<class M>
        static auto execute() {
            return function_lambda_ptr<M>::template call_1<T, U...>;
        }
    };

    template<class T, class... U>
    class function_lam<false, T, U...> {
    public:
        template<class M>
        static auto execute() {
            return function_lambda_ptr<M>::template call_2<T>;
        }
    };

    template<class T>
    class function_lambda;

    template<class T, class... U>
    class function_lambda<T(U...)> {
    public:
        static constexpr int _size = sizeof...(U);
    public:
        void* _data = nullptr;
        void* (*fun)(void*, void*) = nullptr;


        function_lambda() : _data(nullptr), fun(nullptr) {};
        template<class... M>
        function_lambda(const stl::anybind<M...>& bind) {
            this->_data = new function_lambda_ptr<stl::anybind<M...>>(bind);
            this->fun = stl::function_lam<_size, T, U...>::template execute<stl::anybind<M...>>();
        };
        template<class M>
        function_lambda(const M& lambda) {
            this->_data = new function_lambda_ptr<M>(lambda);
            this->fun = stl::function_lam<_size, T, U...>::template execute<M>();
        };

        template<class M>
        function_lambda(M* ptr) {
            this->_data = new function_lambda_ptr<M*>(ptr);
            this->fun = stl::function_lam<_size, T, U...>::template execute<M*>();
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

    };

    //-----------------------------------------------------------------------------------------
    //function

    template<class T>
    class function;

    template<class T, class... U>
    class function<T(U...)> {
    public:
        using _bind = function_bind<T(U...)>;
        using _lambda = function_lambda<T(U...)>;
    public:
        bool is_bind = false;
        _bind* _bind_ptr = nullptr;
        _lambda* _lambda_ptr = nullptr;

        function() : _bind_ptr(nullptr), _lambda_ptr(nullptr) {};
        template<class... M>
        function(const stl::anybind<M...>& bind) :_bind_ptr(new _bind(bind)), is_bind(true) {};
        template<class M>
        function(const M& lambda_ptr) : _lambda_ptr(new _lambda(lambda_ptr)) {};
        template<class M>
        function(M* lambda_ptr) : _lambda_ptr(new _lambda(lambda_ptr)) {};

        T operator()(U... args) {
            //如果是绑定函数
            if (is_bind) { return (*_bind_ptr)(args...); }
            return (*_lambda_ptr)(args...);
        };

        ~function() {
            if (is_bind) {
                delete _bind_ptr;
            }
            else {
                delete _lambda_ptr;
            }
        }
    };


}