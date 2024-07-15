//
// Created by wave on 2024/7/13.
//

#ifndef LOTUS_NOCOPYABLE_H
#define LOTUS_NOCOPYABLE_H

class Noncopyable {
public:
    Noncopyable(const Noncopyable& temp)= delete;
    Noncopyable& operator=(const Noncopyable&)=delete;
protected:
    Noncopyable()=default;
    ~Noncopyable()=default;

};




#endif //LOTUS_NOCOPYABLE_H
