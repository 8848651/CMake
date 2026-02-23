#pragma once
#include <functional>
#include <memory>
#include <iostream>


class eventloop;

class channel{
public:
    channel(int _socketfd,eventloop& _loop):socketfd(_socketfd),loop(_loop) {};


public:
    int socketfd;
    eventloop& loop;
};