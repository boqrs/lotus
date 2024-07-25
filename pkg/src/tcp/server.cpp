//
// Created by wave on 2024/7/25.
//

#include <functional>
#include <string.h>

#include "../../include/tcp/Server.h"
#include "../../include/utils/Logger.h"
#include "../../include/tcp/Connection.h"

//TODO: 迁移到utils或者event上面
static EventLoop *CheckLoopNotNull(EventLoop *loop)
{
    if (loop == nullptr)
    {
        LOG_FATAL("%s:%s:%d mainLoop is null!\n", __FILE__, __FUNCTION__, __LINE__);
    }
    return loop;
}

TcpServer::TcpServer(EventLoop *loop, const InetAddress &listenAddr, const std::string &nameArg,
                     TcpServer::Option option)
                     :loop_(CheckLoopNotNull(loop))
                     ,port_(listenAddr.toIpPort())
                     ,name_(nameArg)
                     ,acceptor_(new Acceptor(loop, listenAddr, option==kReusePort))
                     ,threadPool_(new EventLoopThreadPool(loop, nameArg))
                     ,connectionCallback_()
                     ,messageCallback_()
                     ,nextConnId_(1)
                     ,started_(0){
    acceptor_->setNewConnectionCallback(std::bind(&TcpServer::newConnection, this, std::placeholders::_1, std::placeholders::_2));
}

TcpServer::~TcpServer() {
    for (auto& item: connections_){
        TcpConnectionPtr conn(item.second);
        item.second.reset();

        conn->getLoop()->runInLoop(
                std::bind(&TcpConnection::connectDestroyed, conn));
    }
}

void TcpServer::setThreadNum(int numThreads) {
    threadPool_->setThreadNum(numThreads);
}

void TcpServer::start() {
    if (started_++ == 0){
        threadPool_->start(threadInitCallback_);
        loop_->runInLoop(std::bind(&Acceptor::listen, acceptor_.get()));//为啥connection要用shearedfromthis
    }
}

void TcpServer::newConnection(int socketfd, const InetAddress &peerAddr) {
    EventLoop *ioLoop = threadPool_->getNextLoop();
    char buf[64] = {0};
    snprintf(buf, sizeof buf, "-%s#%d", ipPort_.c_str(), nextConnId_);
    ++nextConnId_;
    std::string connName = name_ + buf;

    LOG_INFO("TcpServer::newConnection [%s] - new connection [%s] from %s\n",
             name_.c_str(), connName.c_str(), peerAddr.toIpPort().c_str());

    sockaddr_in local;
    ::memset(&local, 0, sizeof(local));
    socklen_t addrlen = sizeof(local);
    if(::getsockname(socketfd, (sockaddr *)&local, &addrlen) < 0)
    {
        LOG_ERROR("sockets::getLocalAddr");
    }
    InetAddress localAddr(local);
    TcpConnectionPtr conn(new TcpConnection(ioLoop,
                                            connName,
                                            sockfd,
                                            localAddr,
                                            peerAddr));
    connections_[connName] = conn;

    conn->setConnectionCallback(connectionCallback_);
    conn->setMessageCallback(messageCallback_);
    conn->setWriteCompleteCallback(writeCompleteCallback_);

    conn->setCloseCallback(
            std::bind(&TcpServer::removeConnection, this, std::placeholders::_1)); //占位符号的使用,这样对象已经，占位符号都和函数绑定

    ioLoop->runInLoop(
            std::bind(&TcpConnection::connectEstablished, conn));
}

void TcpServer::removeConnection(const TcpConnectionPtr &conn)
{
    loop_->runInLoop(
            std::bind(&TcpServer::removeConnectionInLoop, this, conn));
}

void TcpServer::removeConnectionInLoop(const TcpConnectionPtr &conn)
{
    LOG_INFO("TcpServer::removeConnectionInLoop [%s] - connection %s\n",
             name_.c_str(), conn->name().c_str());

    connections_.erase(conn->name());
    EventLoop *ioLoop = conn->getLoop();
    ioLoop->queueInLoop(
            std::bind(&TcpConnection::connectDestroyed, conn));
}