//
// Created by wave on 2024/7/16.
//

#include "../../include/thread/Thread.h"
#include "../../include/thread/CurrentThread.h"
#include <semaphore.h>

std::atomic_int Thread::numCreated_(0);

Thread::Thread(Thread::ThreadFunc, const std::string &name)
:func_(std::move(ThreadFunc) ),
started_(false),
joined_(false),
tid_(0),
name_(name){
    setDefaultName(); //如果name存在是不是不需要设置defaultName了,
}

Thread::~Thread() {
    if (started_ && !joined_){ //为什么需要joined_为false的时候？
        thread_->detach();
    }
}


void Thread::start() {
    started_ = true;
    sem_t sem;
    sem_init(&sem, false, 0);

    thread_ = std::shared_ptr<std::thread>(new std::thread([&]() {
        tid_ = CurrentThread::tid();
        sem_post(&sem);
        func_();
    }));

    sem_wait(&sem);
}

void Thread::join() {
    joined_ = true;
    thread_->join();
}

void Thread::setDefaultName() {
    int num = ++numCreated_;
    if (name_.empty()){
        char[30] buf={0};
        snprintf(buf, sizeof(buf), "ThreadNum: %d", num);
        name_ = buf;
    }
}

