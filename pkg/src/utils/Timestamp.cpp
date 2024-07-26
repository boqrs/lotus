//
// Created by wave on 2024/7/13.
//
#include <time.h>
#include "../../include/utils/Timestamp.h"

#define defaultBufferLen 1024
#define defaultYear 1900
#define defaultMonth 1

Timestamp::Timestamp():microSecondsSinceEpoch_(0){};

Timestamp::Timestamp(int64_t microSecondsSinceEpoch):microSecondsSinceEpoch_(microSecondsSinceEpoch) {};


Timestamp Timestamp::now() {
    return Timestamp(time(nullptr));
}

std::string Timestamp::toString() const {
    char buf[defaultBufferLen];
    tm* nowTime = localtime(&microSecondsSinceEpoch_);
    snprintf(buf, defaultBufferLen, "%4d/%02d/%02d %02d::%02d::%02d",
             nowTime->tm_year+defaultYear,
             nowTime->tm_mon+defaultMonth,
             nowTime->tm_mday,
             nowTime->tm_hour,
             nowTime->tm_min,
             nowTime->tm_sec);
}