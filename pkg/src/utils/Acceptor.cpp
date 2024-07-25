//
// Created by wave on 2024/7/17.
//
#include <sys/types.h>
#include <sys/socket.h>
#include <errno.h>
#include <unistd.h>

#include "../../include/utils/Acceptor.h"
#include "../../include/utils/Logger.h"
#include "../../include/utils/InetAddress.h"

static int createNonblocking()
{
    int sockfd = ::socket(AF_INET, SOCK_STREAM | SOCK_NONBLOCK | SOCK_CLOEXEC, IPPROTO_TCP);
    if (sockfd < 0)
    {
        LOG_FATAL("%s:%s:%d listen socket create err:%d\n", __FILE__, __FUNCTION__, __LINE__, errno);
    }
    return sockfd;
}

Acceptor::Acceptor(EventLoop *loop, const InetAddress &listenAddr, bool reusePort):
loop_(loop),
acceptorSocket_(createNonblocking()),
acceptorChannel_(loop, acceptorSocket_.fd()),
listenning_(false)
{
    acceptorSocket_.setReusePort(true);
    acceptorSocket_.setReuseAddr(true);
    acceptorSocket_.bindAddress(listenAddr);
    acceptorChannel_.setReadCallback(std::bind(&Acceptor::handleRead, this));
}

Acceptor::~Acceptor() {
    acceptorChannel_.disableAll();
    acceptorChannel_.remove();
}

void Acceptor::listen() {
    listenning_ = true;
    acceptorSocket_.listen();
    acceptorChannel_.enableReading();
}

void Acceptor::handleRead() {
    InetAddress peerAddress;
    int confd = acceptorSocket_.accept(&peerAddress);
    if (confd >=0){
        if (NewConnectionCallback_){
            NewConnectionCallback_(confd, peerAddress);
        } else{
            LOG_ERROR("%s:%s:%d  connection callback not register error\n", __FILE__, __FUNCTION__, __LINE__);
            ::close(confd);
        }
    } else{
        LOG_ERROR("%s:%s:%d accept err:%d\n", __FILE__, __FUNCTION__, __LINE__, errno);
        if (errno == EMFILE)
        {
            LOG_ERROR("%s:%s:%d sockfd reached limit\n", __FILE__, __FUNCTION__, __LINE__);
        }
    }
}