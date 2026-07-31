#pragma once
#include <type_traits>
#include <cstddef>
#include <meta/template.h>
#include <complex>
namespace stl {

    //迭代法
    template<typename T, size_t U>
    void dit_butterfly_iterate(std::array<std::complex<T>, U>& array){
        size_t size=array.size();
        for(int len=2;len<=size;len=len*2){
            std::complex<T> wn = std::polar(1.0, -2*M_PI/len);
            for(int i=0;i<size/len;i++){
                std::complex<T> w = 1.0;
                for(int j=i*len;j<i*len+len/2;j++){
                    auto a = array[j];
                    auto b = array[j+len/2];
                    array[j] = a + b * w;
                    array[j+len/2] = a - b * w;
                    w *= wn;
                }
            }
        }
    }

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
            std::complex<T> w = std::polar(1.0, -2.0 * M_PI * temp[r] / (U / N));

            auto datae = array[i];
            auto datao = array[i + N];
            array[i]     = datae + datao * w;
            array[i + N] = datae - datao * w;
        }
    };



    //Q 为FFT长度 M为起始位置
    template<size_t Q,size_t M,typename T,size_t U>
    typename stl::void_type<Q==2>::type dif_butterfly_step(std::array<std::complex<T>, U>& array){
        auto datae = array[M];
        auto datao = array[M+1];
        array[M] = datae + datao;
        array[M+1] = datae - datao;
    }

    template<size_t Q,size_t M,typename T,size_t U>
    typename stl::void_type<Q!=2>::type dif_butterfly_step(std::array<std::complex<T>, U>& array){
        std::complex<double> wn = std::polar(1.0,-2.0*M_PI/Q);
        std::complex<double> w = 1.0;
        for(int i=M;i<M+Q/2;i++){
            auto datae = array[i];
            auto datao = array[i+Q/2];
            array[i] = datae + datao;
            array[i+Q/2] = (datae - datao)*w;
            w*=wn;
        }
        dif_butterfly_step<Q/2,M>(array);
        dif_butterfly_step<Q/2,M+Q/2>(array);
    }



}