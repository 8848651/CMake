#pragma once
#include <functional>

namespace stl {

    template<typename T>
    class function;

    template<typename T, typename... U>
    class function<T(U...)> {
    public:

        template<typename R>
        function(T(R::* temp)(U...), R* r) :function([temp, r](U... u) {return r->temp(std::forward<U>(u)...);}) {}

        function(T(*temp)(U...)) :function([temp](U... u) {return temp(std::forward<U>(u)...);}) {}

        function() :function([](U... u) {throw std::bad_function_call{};}) {}

        template<typename R,typename = decltype(std::declval<R>()(std::declval<U>()...))>
        function(R temp) {
            funimpl = new R{ std::move(temp) };
            destroy = function<T(U...)>::template fundestory<R>;
            execute = function<T(U...)>::template funexecute<R>;
        }

        

        T operator()(U... u) {
            return execute(funimpl, std::forward<U>(u)...);
        };

        ~function() {
            destroy(funimpl);
        }

    private:
        void* funimpl = nullptr;
        void (*destroy)(void*);
        T(*execute)(void*, U...);

        template<typename R>
        static void fundestory(void* ptr) {
            auto funimpl = static_cast<R*>(ptr);
            delete funimpl;
        };

        template<typename R>
        static T funexecute(void* ptr, U... u) {
            auto funimpl = static_cast<R*>(ptr);
            return (*(funimpl))(std::forward<U>(u)...);
        };







    };
}