//
// Created by wave on 2024/7/15.
//

#include <errno.h>
#include <unistd.h>
#include <string.h>

#include "../../include/poller/EpollPoller.h"
#include "../../include/utils/Logger.h"
#include "../../include/channel/Channel.h"


const int kNew = -1;    // 某个channel还没添加至Poller
const int kAdded = 1;   // 某个channel已经添加至Poller
const int kDeleted = 2; // 某个channel已经从Poller删除

EpollPoller::EpollPoller(EventLoop *loop):
        Poller(loop),
        epollfd_(::epoll_create1(EPOLL_CLOEXEC)),
        events_(kInitEventListSize)
{
    if (epollfd_ < 0)
    {
        LOG_FATAL("epoll_create error:%d \n", errno);
    }
}


EpollPoller::~EpollPoller() {
    ::close(epollfd_);
}

Timestamp EpollPoller::poll(int timeoutMs, Poller::ChannelList *activeChannels) {
    LOG_INFO("func=%s => fd total count:%lu\n", __FUNCTION__, channels_.size());

    int numEvents = ::epoll_wait(epollfd_, &*events_.begin(), static_cast<int>(events_.size()), timeoutMs);
    int saveErrno = errno; //TODO:
    Timestamp now = Timestamp::now();

    if (numEvents > 0)
    {
        LOG_INFO("%d events happend\n", numEvents); // LOG_DEBUG最合理
        fillActiveChannels(numEvents, activeChannels);
        if (numEvents == events_.size())
        {
            events_.resize(events_.size() * 2); //这是把events列表扩容为2倍
        }
    }
    else if (numEvents == 0)
    {
        LOG_DEBUG("%s timeout!\n", __FUNCTION__);
    }
    else
    {
        if (saveErrno != EINTR)
        {
            errno = saveErrno;
            LOG_ERROR("EPollPoller::poll() error!");
        }
    }
    return now;
}


void EpollPoller::updateChannel(Channel *channel) {
    const int index = channel->index(); //这里用一个状态标志位是不是更加合适一些
    LOG_INFO("func=%s => fd=%d events=%d index=%d\n", __FUNCTION__, channel->fd(), channel->events(), index);

    if (index == kNew || index == kDeleted) //TODO：为什么把kDeleted也要放在这里
    {
        if (index == kNew)
        {
            int fd = channel->fd();
            channels_[fd] = channel;
        }
        else // index == kAdd
        {
        }
        channel->set_index(kAdded);
        update(EPOLL_CTL_ADD, channel);
    } else{
        int fd = channel->fd();
        if (channel->isNoneEvent())
        {
            update(EPOLL_CTL_DEL, channel);
            channel->set_index(kDeleted);
        }
        else
        {
            update(EPOLL_CTL_MOD, channel);
        }
    }
}


void EpollPoller::removeChannel(Channel *channel) {
    int fd = channel->fd();
    channels_.erase(fd);

    LOG_INFO("func=%s => fd=%d\n", __FUNCTION__, fd);
    int index = channel->index();
    if (index == kAdded)
    {
        update(EPOLL_CTL_DEL, channel);
    }
    channel->set_index(kNew);
}

void EpollPoller::fillActiveChannels(int numEvents, Poller::ChannelList *activeChannels) const {
    for (int i =0; i < numEvents--; i++){
        Channel* channel = static_cast<Channel*>(events_[i].data.ptr);
        channel->set_revents(events_[i].events);
        activeChannels->push_back(channel);
    }
}

void EpollPoller::update(int operation, Channel *channel) {
    epoll_event event;
    ::memset(&event, 0, sizeof(event));

    int fd = channel->fd();

    event.events = channel->events();
    event.data.fd = fd;
    event.data.ptr = channel;

    if (::epoll_ctl(epollfd_, operation, fd, &event) < 0){
        if (operation == EPOLL_CTL_DEL)
        {
            LOG_ERROR("epoll_ctl del error:%d\n", errno);
        }
        else
        {
            LOG_FATAL("epoll_ctl add/mod error:%d\n", errno);
        }
    }
}