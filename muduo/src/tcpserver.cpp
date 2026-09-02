#include "tcpserver.h"


tcpserver::tcpserver(messagecallback messagecallback)
    :baseloop_(std::make_shared<eventloop>())
    ,connection_()
    ,accepto_(){
    baseloop_->init();
    accepto_.init(baseloop_);
    accepto_.setcallback([&](int acceptfd){newconnect(acceptfd);});
    messagecallback_ = messagecallback;
    baseloop_->loop();
};

void tcpserver::newconnect(int acceptfd){
    //std::shared_ptr<eventloop> loop = thread.geteventloopptr();
    //std::cout<<"执行"<<std::endl;
    std::shared_ptr<channel> newchannel=std::make_shared<channel>(acceptfd,baseloop_);
    newchannel->setreadcallback([=](){messagecallback_(*(newchannel.get()));});
    baseloop_->tosubmittask([=](){
        std::cout << "对象存活：" << newchannel.use_count() << std::endl;
        newchannel->update();
    });
}



