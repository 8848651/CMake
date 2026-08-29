// #pragma once
// #include <algo/fft.h>
// #include <meta/binaryreversal.h>
// #include <random>
// #include <array>
// #include <complex>
// #include <iostream>

// namespace test{

//     static void butterfly_test(){
//         constexpr static size_t tp = 8;
//         auto temp = stl::bitrev_table<tp,typename stl::makeindexqueue<tp>::queuedata>::table.begin();
//         constexpr double PI = 3.14159265358979323846;
//         std::array<std::complex<double>, tp> arr;

//         //采样频率是1秒中转了几圈，规定了相邻两个采样点的时间
//         double Fs = 16; 

//         for (size_t i = 0; i < tp; ++i) {

//             double t = static_cast<double>(i) / Fs; 
//             double signal = std::sin(2 * PI * 2.0 * t);  

//             arr[i] = std::complex<double>(signal, 0.0);
//             std::cout << signal << " ";
//         }
//         std::cout << std::endl;


//         for(int i=0;i < tp; i++){
//             if(temp[i] >= i){
//                 std::swap(arr[temp[i]] , arr[i]);
//             }
//         }

//         stl::dit_butterfly_iterate(arr);

//         for(int i=0;i<tp;i++){
//             double magnitude = std::abs(arr[i]);
//             if (magnitude < 1e-12) {
//                 magnitude = 0.0;
//             }
//             std::cout << magnitude << " ";
//         }
//         std::cout<<std::endl;
//     }
// }