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

    template<typename T>
    class bindimpl;

    //被绑定阶段只能是值绑定
    // bind captures by value only. No dangling references by design.
    template<typename T, typename... U, typename... Args>
    auto bind(T(*func)(U...), Args... args) {
        using type=typename stl::typequeueassisted<stl::typequeue<U...>,stl::typequeue<Args...>>::type;
        stl::bindimpl<type> bindimplinstance;
        return bindimplinstance.recall([=](U&&... u) mutable {return std::forward<T>(func(std::forward<U&&>(u)...));}, std::forward<Args&&>(args)...);
    };

    //--------------------------------------------------------------------------------------

    template<typename... T>
    class bindimpl<typequeue<T...>> {
    public:
        template<typename R, typename... Args>
        auto recall(R callblack, Args&&... args) {
            using queuedata = typename stl::makeindexqueue<sizeof...(Args)>::queuedata;
            bindassisted<queuedata, Args...>  bindassistedinstance(std::forward<Args&&>(args)...);
            return [=](T&&... t) mutable {return bindassistedinstance(callblack, std::forward<T&&>(t)...);};
        };
    };


    template<size_t... Is, typename... Args>
    class bindassisted<indexqueue<Is...>, Args...> {
    public:
        parametertype<stl::placeholders(Args...)> parameter;
        bindassisted(Args&&... args) :parameter(std::forward<Args>(args)...) {};
        bindassisted(const bindassisted& other) : parameter(other.parameter) {};
        template<typename R, typename... U>
        auto operator()(R callblack, U&&... u) {
            auto bindparameter = parameter(std::forward<U&&>(u)...);
            return callblack(std::forward<typename tupleelement<Is, decltype(bindparameter)>::type>(tuplefindelement<Is>(bindparameter))...);
        }
    };


    //--------------------------------------------------------------------------------------

    template<typename T, typename... U, typename MyClass, typename... Args>
    auto bind(T(MyClass::* func)(U...), MyClass* ptr, Args... args) {
        return 0;
    };

    template<typename T, typename... U, typename... Args>
    auto bind(stl::function<T(U...)> fun, Args... args) {
        return 0;
    };















}
