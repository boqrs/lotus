//
// Created by wave on 2024/7/25.
//

#ifndef LOTUS_SERVER_H
#define LOTUS_SERVER_H
#include <functional>
#include <string>
#include <memory>
#include <atomic>
#include <unordered_map>

#include "../channel/Channel.h"

#include "../event/EventLoop.h"
#include "../utils/Acceptor.h"
#include "../utils/InetAddress.h"
#include "../utils/Noncopyable.h"
#include "../event/EventLoopThreadPool.h"
#include "../utils/Callbacks.h"
#include "../utils/Buffer.h"

#include "Connection.h"

class TcpServer {
public:
    using ThreadInitCallback = std::function<void(EventLoop *)>;
    enum Option {
        kNoReusePort,
        kReusePort,
    };

    TcpServer(EventLoop *loop,
              const InetAddress& listenAddr,
              const std::string& nameArg,
              Option option=kNoReusePort);
    ~TcpServer();
    void setThreadInitCallback(const ThreadInitCallback& cb){threadInitCallback_=cb;};
    void setConnectionCallback(const ConnectionCallback&  cb){connectionCallback_ = cb;};
    void setMessageCallback(const MessageCallback& cb){messageCallback_ = cb;};
    void setWriteCompleteCallback(const WriteCompleteCallback& cb){writeCompleteCallback_ = cb;};

    void setThreadNum(int numThreads);
    void start();

private:
    void newConnection(int socketfd, const InetAddress& peerAddr);
    void removeConnection(const TcpConnectionPtr& conn);
    void removeConnectionInLoop(const TcpConnectionPtr& conn);
    using ConnectionMap = std::unordered_map<std::string, TcpConnectionPtr>;

    EventLoop* loop_; //baseLoop
    const std::string ipPort_;
    const std::string name_;

    std::unique_ptr<Acceptor> acceptor_;
    std::shared_ptr<EventLoopThreadPool> threadPool_;

    ConnectionCallback  connectionCallback_;
    MessageCallback  messageCallback_;
    WriteCompleteCallback writeCompleteCallback_;
    ThreadInitCallback threadInitCallback_;

    std::atomic_int started_;
    int nextConnId_;
    ConnectionMap connections_;
};
#endif //LOTUS_SERVER_H
