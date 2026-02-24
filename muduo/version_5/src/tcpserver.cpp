#include "tcpserver.h"


tcpserver::tcpserver()
    :baseloop_()
    ,accepto(*(baseloop_.get()))
    ,thread(){
    accepto.setcallback([&](int acceptfd){newconnect(acceptfd);});
};

void tcpserver::newconnect(int acceptfd){
    std::shared_ptr<eventloop> loop = thread.eventloop_;
    std::shared_ptr<channel> connectchannel=std::make_shared<channel>(acceptfd,loop);
    connectchannel->setreadcallback(readcallback);
    loop->tosubmittask([&](){connectchannel->update();});
}



