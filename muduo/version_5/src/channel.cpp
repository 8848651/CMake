#include <memory>
#include "channel.h"
#include "eventloop.h"


void channel::updateForEpoll() {
    loop->update(shared_from_this());
};

void channel::test() {
    std::cout << 456654 << std::endl;
};