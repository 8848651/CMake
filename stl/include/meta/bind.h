#pragma once
#include <iostream>
#include "tuple.h"
#include "indexqueue.h"
#include "typereplace.h"
#include "function.h"
#include "typedifference.h"
namespace stl {

    /**
    * @brief 此bind无法绑定入参为右值引用，如果要使用右值请使用const T&
    * @return 返回一个lambda表达式
    */

    enum class placeholders {
        _1, _2, _3, _4, _5, _6, _7, _8, _9, _10
    };

    template<typename... T>
    class bindassisted;

    template<typename T, typename U>
    class bindimpl;

    template<typename T, typename U, typename R>
    class bindtypeimpl;

    //添加一个可以绑定引用的版本 
    //Atype 新绑定类型
    //Ltype 入参类型
    template<typename T, typename... U, typename... Args>
    auto bindreference(T(*func)(U...), Args&&... data) {
        using Atype = typename stl::typequeuereferenceassisted<stl::typequeue<U...>, stl::typequeue<Args...>>::type;
        using Ltype = typename stl::typequeueassisted<stl::typequeue<U...>, Atype>::type;
        bindimpl<Atype, Ltype> bindimplinstance;
        return bindimplinstance.recall([=](U... u) mutable {return std::forward<T>(func(std::forward<U>(u)...));}, std::forward<Args>(data)...);
    };


    template<typename... T, typename... U>
    class bindimpl<typequeue<T...>, typequeue<U...>> {
    public:
        template<typename R, typename... Args>
        auto recall(R callblack, Args&&... data) {
            using queuedata = typename stl::makeindexqueue<sizeof...(Args)>::queuedata;
            return [=](U&&... args) mutable {
                stl::tuple<T...> data(std::forward<Args>(data)...);
                stl::tuple<U...> args(std::forward<U>(args)...);
                parametertype <stl::placeholders, stl::typequeue<T...>, stl::typequeue<U...>> temp(data, args);
                return bindrecall(callblack, temp.recell(), queuedata());
                };
        };

        template<typename R, typename... T, size_t... Is>
        auto bindrecall(R callback, tuple<T...>&& t, indexqueue<Is...>) {
            return callblack(tuplefindelement<Is>(t)...);
        };
    };

}
