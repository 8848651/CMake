#pragma once
#include <iostream>
#include "tuple.h"
#include "indexqueue.h"
#include "typereplace.h"
#include "function.h"
#include "typedifference.h"
#include "typesubstitute.h"
namespace stl {

    /**
    * @brief 此bind无法绑定入参为右值引用，如果要使用右值请使用const T&
    * @return 返回一个lambda表达式
    */

    enum class placeholders {
        _1, _2, _3, _4, _5, _6, _7, _8, _9, _10
    };

    template<typename... T>
    class bindimpl;

    template<typename T>
    T bindtype() {};

    template<typename T, typename... U, typename... Args>
    auto bind(T(*func)(U...), Args&&... data) {
        //Atype 新绑定类型  Ltype 入参类型
        using Utype = stl::typequeue<U...>;
        using Atype = typename stl::typequeuereferenceassisted<Utype, stl::typequeue<Args...>>::type;
        using Ltype = typename stl::typequeueassisted<Utype, Atype>::type;
        return bindimpl<T, Atype, Ltype, Utype>::recall(
            [=](U... u) mutable -> decltype(bindtype<T>()) {return func(std::forward<U>(u)...);},
            std::forward<Args>(data)...
        );
    };

    template<typename Re, typename... T, typename... U, typename... Utype>
    class bindimpl<Re, typequeue<T...>, typequeue<U...>, typequeue<Utype...>> {
    public:
        template<typename R, typename... Args>
        static auto recall(R callback, Args&&... data) {
            using queuedata = typename stl::makeindexqueue<sizeof...(Args)>::queuedata;
            return [&, _callback = callback](typename stl::bindadd_reference<U>::type... args) mutable -> decltype(bindtype<Re>()) {
                stl::tuple<T...> _data(std::forward<Args>(data)...);
                stl::tuple<U...> _args(std::forward<typename stl::bindadd_reference<U>::type>(args)...);
                //stl::parametertype<stl::placeholders, stl::typequeue<T...>, stl::typequeue<U...>> temp(_data, _args);
                stl::argumenttype<stl::typequeue<Utype...>, stl::typequeue<T...>, stl::typequeue<U...>> temp(_data, _args);
                // l<typequeue<Utype...>> _1;
                // l<typequeue<T...>> _2;
                // l<typequeue<U...>> _3;
                temp.recell();
                return bindrecall(_callback, temp.recell(), queuedata());
                //return Re{};
                };
        };
    private:
        template<typename R, typename... Q, size_t... Is>
        static Re bindrecall(R callback, tuple<Q...>&& t, indexqueue<Is...>) {
            return callback(tuplefindelement<Is>(t)...);
        };
    };

}
