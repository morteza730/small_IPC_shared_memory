/**
 * @file testNotifier.cpp
 * @brief Unit tests for the Notifer class.
 */

#include <gtest/gtest.h>
#include "notifier.h"
#include <thread>
#include <chrono>

TEST(NotifierTest, Initialization) {
    Notifer notifier;
    EXPECT_TRUE(notifier.init(0xfa00));
}

TEST(NotifierTest, NotifyWait) {
    Notifer notifier;
    notifier.init(0xfa00);

    // Start a thread that will wait
    std::thread waiter([&notifier]() {
        notifier.wait();
    });

    // Give some time for the thread to start waiting
    std::this_thread::sleep_for(std::chrono::milliseconds(100));

    // Notify the waiting thread
    notifier.notify();

    // Join the thread to ensure it has finished
    waiter.join();
}