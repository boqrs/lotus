//
// Created by wave on 2024/7/26.
//

#include "../../include/poller/Poller.h"
#include "../../include/poller/EpollPoller.h"

Poller *Poller::newDefaultPoller(EventLoop *loop)
{
    if (::getenv("LOTUS_USE_POLL"))
    {
        return nullptr;
    }
    return new EpollPoller(loop);
}