#pragma once
#include "tuple.h"
#include "construct.h"
#include "thread.h"
namespace stl {

    template<class T>
    class TT;

    template<int... Is>
    class TT<IntList<Is...>> {
    public:
        template<class... Args>
        static int run(Args... args) {
            static constexpr int size = sizeof...(Args) - 1;
            Tuple<Args...> _args(args...);
            int a = TupleFindType<4, int>::find(_args.base);
            std::cout << "a=" << a << std::endl;
            return 0;
        };
    };

    template<class... Args>
    void PP(Args... args) {
        static constexpr int size = sizeof...(Args) - 1;
        TT<class AssistedQueue<size>::QueueData>::run(args...);
    };








}
