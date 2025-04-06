#pragma once
#include "bind.h"
namespace stl {

    
    template<class T>
    class callbase_function;

    template<bool T>
    class callbase_function_;

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


    //anybind版本
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
            this->fun=stl::function_fun<_size, T, U...>::template execute<M...>();
        };

        template<class... M>
        T operator()(M... args) {
            Tuple<M...>* _args = new Tuple<M...>(args...);
            void* _temp = fun(_data, _args);
            delete _args;
            return is_type<T>::_cast(_temp);
        };

        T operator()() {
            void* _temp = fun(_data,nullptr);
            return is_type<T>::_cast(_temp);
        };

    };


}