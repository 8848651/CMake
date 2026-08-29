#include "channel.h"
#include "eventloop.h"

namespace muduo {

    void channel::update() {
        loop_.lock()->update(shared_from_this());
    }

}
