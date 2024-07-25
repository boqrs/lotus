//
// Created by wave on 2024/7/15.
//

#ifndef LOTUS_THREAD_H
#define LOTUS_THREAD_H
#include <functional>
#include <thread>
#include <memory>
#include <unistd.h>
#include <string>
#include <atomic>

#include "../include/utils/Noncopyable.h"

class Thread : Noncopyable
{
public:
    using ThreadFunc = std::function<void()>;
    explicit Thread(ThreadFunc, const std::string &name = std::string());//构造函数explicit避免银式的类型转换和复值初始化,必须使用显式
    //的构造,name如果不存在就是空字符串.
    ~Thread();

    void start();
    void join();

    bool started() { return started_; }
    pid_t tid() const { return tid_; }
    const std::string&name()const{return name_;}
    static int numCreated(){return numCreated_;}

private:
    void setDefaultName();
    bool started_;
    bool joined_;
    std::shared_ptr<std::thread> thread_;
    pid_t tid_;
    ThreadFunc func_;
    std::string name_;
    static std::atomic_int numCreated_;
};

#endif //LOTUS_THREAD_H
