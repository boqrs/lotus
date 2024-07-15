//
// Created by wave on 2024/7/13.
//

#ifndef LOTUS_TIMESTAMP_H
#define LOTUS_TIMESTAMP_H
class Timestamp {
public:
    Timestamp();
    explicit Timestamp(int64_t microSecondsSinceEpoch);
    static Timestamp now();
    std::string toString() const;
private:
    int64_t microSecondsSinceEpoch_;
};

#endif //LOTUS_TIMESTAMP_H
