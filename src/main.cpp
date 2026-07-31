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


auto FFT(){
    
}




int main() {
    constexpr static size_t tp = 8;
    auto temp = stl::bitrev_table<tp,typename stl::makeindexqueue<tp>::queuedata>::table.begin();

    std::random_device rd;
    
    std::mt19937 gen{rd()};
    
    std::uniform_int_distribution<int> dis(1, 100);

    std::array<std::complex<double>, tp> arr;
    std::array<std::complex<double>, tp> arr_t;
    for(int i=0;i<tp;i++){
        auto temp=dis(gen);
        arr[i]=std::complex<double>(temp, 0);
        arr_t[i]=std::complex<double>(temp, 0);
        std::cout<< arr[i] <<" ";
    }
    std::cout<<std::endl;

    stl::dit_butterfly_step<tp,0,1>(arr_t);


    for(int i=0;i < tp; i++){
        if(temp[i] >= i){
            std::swap(arr[temp[i]] , arr[i]);
            std::swap(arr_t[temp[i]] , arr_t[i]);
        }
    }

    for(int len=2;len<=tp;len=len*2){
        std::complex<double> wn = std::polar(1.0, -2*M_PI/len);
        for(int i=0;i<tp/len;i++){
            std::complex<double> w = 1.0;
            for(int j=i*len;j<i*len+len/2;j++){
                auto a = arr[j];
                auto b = arr[j+len/2];
                arr[j] = a + b * w;
                arr[j+len/2] = a - b * w;
                w *= wn;
            }
        }
    }

    for(int i=0;i<tp;i++){
        std::cout<<arr[i]<<" ";
    }
    std::cout<<std::endl;

        for(int i=0;i<tp;i++){
        std::cout<<arr_t[i]<<" ";
    }
    std::cout<<std::endl;



    





}
