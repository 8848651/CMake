#include <iostream>
#include <meta/tuple.h>
#include <meta/bind.h>
#include <meta/binaryreversal.h>
#include <meta/function.h>
#include <stl/fft.h>
#include "../muduo/version_5/include/version.h"

#include <random>
#include <array>
#include <complex>



int main() {
    constexpr static size_t tp = 8;
    auto temp = stl::bitrev_table<tp,typename stl::makeindexqueue<tp>::queuedata>::table.begin();

    std::random_device rd;
    
    std::mt19937 gen{rd()};
    
    std::uniform_int_distribution<int> dis(1, 100);

    std::array<std::complex<double>, tp> arr;
    std::array<std::complex<double>, tp> arr_t;
    std::array<std::complex<double>, tp> arr_f;
    for(int i=0;i<tp;i++){
        auto temp=dis(gen);
        arr[i]=std::complex<double>(temp, 0);
        arr_t[i]=std::complex<double>(temp, 0);
        arr_f[i]=std::complex<double>(temp, 0);
        std::cout<< arr[i] <<" ";
    }
    std::cout<<std::endl;

    stl::dit_butterfly_step<tp,0,1>(arr_t);
    stl::dif_butterfly_step<tp,0>(arr_f);


    for(int i=0;i < tp; i++){
        if(temp[i] >= i){
            std::swap(arr[temp[i]] , arr[i]);
            std::swap(arr_t[temp[i]] , arr_t[i]);
            std::swap(arr_f[temp[i]] , arr_f[i]);
        }
    }

    stl::dit_butterfly_iterate(arr);

    for(int i=0;i<tp;i++){
        std::cout<<arr[i]<<" ";
    }
    std::cout<<std::endl;

    for(int i=0;i<tp;i++){
        std::cout<<arr_t[i]<<" ";
    }
    std::cout<<std::endl;

    for(int i=0;i<tp;i++){
        std::cout<<arr_f[i]<<" ";
    }
    std::cout<<std::endl;



    





}
