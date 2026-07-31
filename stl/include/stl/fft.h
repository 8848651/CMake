#pragma once
#include <type_traits>
#include <cstddef>
#include <meta/template.h>
#include <complex>
namespace stl {

    //Q为一次FFT数组的长度，M为起始地址，N为跨度
    template<size_t Q,size_t M,size_t N,typename T, size_t U>
    typename stl::void_type<Q==2>::type dit_butterfly_step(std::array<std::complex<T>, U>& array){
        auto datae = array[M];
        auto datao = array[M + N];
        array[M]     = datae + datao;
        array[M + N] = datae - datao;
    }

    template<size_t Q,size_t M,size_t N,typename T, size_t U>
    typename stl::void_type<Q!=2>::type dit_butterfly_step(std::array<std::complex<T>, U>& array){

        dit_butterfly_step<Q/2,M,N*2>(array);
        dit_butterfly_step<Q/2,M+N,N*2>(array);
        auto temp = stl::bitrev_table<Q/2,typename stl::makeindexqueue<Q/2>::queuedata>::table.begin();
    
        for (size_t i = M; i < array.size(); i = i + N * 2) {
            size_t r=(i-M)/(2*N);
            std::complex<double> w = std::polar(1.0, -2.0 * M_PI * temp[r] / (U / N));

            auto datae = array[i];
            auto datao = array[i + N];
            array[i]     = datae + datao * w;
            array[i + N] = datae - datao * w;
        }
    };


}