#include <gtest/gtest.h>
#include "blocking_queue.hpp"

TEST(BlockingQueueTest, ThreadTest) { 
    BlockingQueue<int> queue;
    queue.Push(100);

    int val = queue.Pop();

    ASSERT_EQ(100, val);
}