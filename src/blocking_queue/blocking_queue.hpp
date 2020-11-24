#ifndef BLOCKING_QUEUE_H
#define BLOCKING_QUEUE_H

#include <mutex>
#include <condition_variable>
#include <deque>

#include "../logger/logger.hpp"

template <typename T>
class BlockingQueue
{
    private:
        std::mutex mutex;
        std::condition_variable conditionVariable;
        std::deque<T> queue;

    public:
        void Push(T&& t);
        T Pop();
        int Count();

};

#include "blocking_queue.cpp"
#endif