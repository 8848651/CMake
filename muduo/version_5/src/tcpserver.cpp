#include "tcpserver.h"


tcpserver::tcpserver(messagecallback messagecallback)
    :baseloop_(std::make_shared<eventloop>())
    ,connection_()
    ,accepto(){
    baseloop_->init();
    accepto.init(baseloop_);
    accepto.setcallback([&](int acceptfd){newconnect(acceptfd);});
    messagecallback_ = messagecallback;
    baseloop_->loop();
};

void tcpserver::newconnect(int acceptfd){
    //std::shared_ptr<eventloop> loop = thread.geteventloopptr();
    std::cout<<"执行"<<std::endl;
    std::shared_ptr<channel> newchannel=std::make_shared<channel>(acceptfd,baseloop_);
    connection_.emplace_back(newchannel);
    newchannel->setreadcallback([&](){messagecallback_(*(newchannel.get()));});
    baseloop_->tosubmittask([&](){
        std::cout<<"添加新链接到epoll"<<std::endl;
        newchannel->update();
    });
}



