//
// Created by wave on 2024/7/24.
//

#ifndef LOTUS_CONNECTION_H
#define LOTUS_CONNECTION_H
#include <memory>
#include <string>
#include <atomic>

#include "../utils/Noncopyable.h"
#include "../utils/InetAddress.h"
#include "../utils/Callbacks.h"
#include "../utils/Buffer.h"
#include "../utils/Timestamp.h"

class Channel;
class EventLoop;
class Socket;

class TcpConnection: Noncopyable, public std::enable_shared_from_this<TcpConnection>{
public:
    TcpConnection(EventLoop* loop,
                  const std::string &nameArg,
                  int socketfd,
                  const InetAddress &localAddr,
                  const InetAddress &peerAddr);

    ~TcpConnection();

    EventLoop* getLoop()const{return loop_;};
    const std::string& getName(){return name_;};
    const InetAddress& getLocal()const{return localAddr_;};
    const InetAddress& getPeer()const{return peerAddr_;};
    bool connected()const{return state_==kConnected;};

    void send(const std::string& buf);
    void shutdown();

    void setConnectionCallback(const ConnectionCallback& cb){
        connectionCallback_ = cb;
    };

    void setMessageCallback(const MessageCallback& cb){
        messageCallback_ = cb;
    };

    void setWriteCompleteCallback(const WriteCompleteCallback& cb){
        writeCompleteCallback_=cb;
    };
    void setCloseCallback(const CloseCallback & cb){
        closeCallback_ = cb;
    };
    void setHighWaterMarkCallback(const HighWaterMarkCallback& cb, size_t highWaterMark){
        highWaterMarkCallback_ = cb; highWaterMark_ = highWaterMark;
    };

private:
    enum StatE{
        kDisconnected,
        kConnecting,
        kConnected,
        kDisconnecting
    };
    void setState(StatE state){state_ = state;};
    void handleRead(Timestamp receiveTime);
    void handleWrite();
    void handleClose();
    void handleError();
    void sendInLoop(const void* data, size_t len);
    void shutDownInLoop();

    EventLoop *loop_;
    const std::string name_;
    std::atomic_int state_;
    bool reading_;
    std::unique_ptr<Channel> channel_;
    std::unique_ptr<Socket> socket_;
    const InetAddress localAddr_;
    const InetAddress peerAddr_;
    ConnectionCallback  connectionCallback_;
    MessageCallback messageCallback_;
    WriteCompleteCallback writeCompleteCallback_;
    HighWaterMarkCallback highWaterMarkCallback_;
    CloseCallback closeCallback_;
    size_t highWaterMark_;

    Buffer inputBuffer_;
    Buffer outBuffer_;
};



#endif //LOTUS_CONNECTION_H
