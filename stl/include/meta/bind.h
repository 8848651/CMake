#pragma once
#include <iostream>
#include "tuple.h"
#include "indexqueue.h"
#include "typereplace.h"
#include "function.h"
#include "typedifference.h"
namespace stl {

    enum class placeholders {
        _1, _2, _3, _4, _5, _6, _7, _8, _9, _10
    };

    template<typename T, typename U>
    class bindassisted;

    template<typename T>
    class bindimpl;

    template<typename T, typename... U, typename... Args>
    auto bind(T(*func)(U...), Args&&... args) {
        using bindtype = typename stl::bindimpl<typequeue<U...>, typequeue<Args...>>::type;
        stl::bindimpl<bindtype> bindimplinstance;
        return bindimplinstance.recall([=](U&&... u) {return fun(std::forward<U>(u)...)}, std::forward<Args>(args)...);
    };

    //--------------------------------------------------------------------------------------

    template<typename... T>
    class bindimpl<typequeue<T...>> {
    public:
        template<typename R, typename... Args>
        auto recall(R callblack, Args&&... args) {
            using queuedata = typename stl::makeindexqueue<sizeof(Args...)>::queuedata;
            bindassisted<queuedata, Args...>  bindassistedinstance{ std::forward<Args>(args)... };
            return [=](T&&... t) {return bindassistedinstance(callblack, std::forward<T>(t)...)};
        };
    };


    template<size_t... Is, typename... Args>
    class bindassisted<indexqueue<Is...>, Args...> {
    public:
        parametertype<stl::placeholders(Args...)> parameter;
        bindassisted(Args... args) :parameter(std::forward<Args>(args)...) {};
        template<typename R, typename... U>
        auto operator()(R callblack, U&&... args) {
            auto bindparameter = parameter(args);
            return callblack(tuplefindelement<Is>(bindparameter)...);
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
