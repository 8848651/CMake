#include "tcpserver.h"


tcpserver::tcpserver()
    :baseloop_(std::make_shared<eventloop>())
    ,accepto(baseloop_)
    ,thread(){
    baseloop_->init();
    accepto.setcallback([&](int acceptfd){newconnect(acceptfd);});
};

tcpserver::tcpserver(messagecallback messagecallback)
    :baseloop_(std::make_shared<eventloop>())
    ,accepto(baseloop_)
    ,thread(){
    baseloop_->init();
    accepto.setcallback([&](int acceptfd){newconnect(acceptfd);});
    messagecallback_ = messagecallback;
};

void tcpserver::setmessagecallback(messagecallback messagecallback){
    messagecallback_ = messagecallback;
};

void tcpserver::newconnect(int acceptfd){
    std::shared_ptr<eventloop> loop = thread.eventloop_;
    std::shared_ptr<channel> newchannel=std::make_shared<channel>(acceptfd,loop);
    newchannel->setreadcallback([&](){messagecallback_(*(newchannel.get()));});
    loop->tosubmittask([&](){newchannel->update();});
}



