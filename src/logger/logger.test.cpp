#include <gtest/gtest.h>
#include <thread>
#include <chrono>
#include "logger.hpp"

TEST(LoggerTest, InitLoggerTest) { 
    // create lot of data with 20+ threds
    Logger::Instance()->LoggerLevel = Logger::LogLevel::INFORMATION;
    Logger::Instance()->Add(Logger::LogType::GENERAL, Logger::LogLevel::INFORMATION, "test log");

    std::this_thread::sleep_for(std::chrono::seconds(3));
}

TEST(LoggerTest, StressTest) { 
    // create lot of data with 20+ threds
}