#include "tcpserver.h"


tcpserver::tcpserver()
    :baseloop_(std::make_shared<eventloop>())
    ,accepto(*(baseloop_.get()))
    ,thread(){
    accepto.setcallback([&](int acceptfd){newconnect(acceptfd);});
};

void tcpserver::setmessagecallback(messagecallback messagecallback){
    messagecallback_ = messagecallback;
};

void tcpserver::newconnect(int acceptfd){
    std::shared_ptr<eventloop> loop = thread.eventloop_;
    std::shared_ptr<channel> connectchannel=std::make_shared<channel>(acceptfd,loop);
    connectchannel->setreadcallback([&](){messagecallback_(*(connectchannel.get()));});
    loop->tosubmittask([&](){connectchannel->update();});
}



