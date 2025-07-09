#pragma once
#include <memory>
#include "channel.h"
#include "eventloop.h"


void channel::updateForEpoll() {
    loop->update(shared_from_this());
};