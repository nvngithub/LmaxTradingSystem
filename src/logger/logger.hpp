#ifndef LOGGER_H
#define LOGGER_H

#include <string>
#include <deque>
#include <thread>
#include <map>
#include <functional>
#include <fstream>
#include <boost/format.hpp>
#include <chrono>
#include <ctime>
#include <sstream>
#include <iomanip>

#include "../blocking_queue/blocking_queue.hpp"

class Logger
{
    public:
        enum LogLevel 
        {
            CRITICAL = 0,
            ERROR,
            WARNING,
            INFORMATION
        };

        enum LogType
        {
            GENERAL,
            GENERAL_TRADING,
            MARKET_DATA,
            ORDER
        };

        static Logger* Instance();
        Logger::LogLevel LoggerLevel;
        void Add(Logger::LogType logType, Logger::LogLevel logLevel, std::string message); // adds log to the queue

    private:

        // make default constructor private for singleton
        Logger()
        {
            this->LoggerLevel = Logger::LogLevel::CRITICAL; //by default only critical logs enabled
        }

        struct LogMessage
        {
            public:
                Logger::LogType logType;
                Logger::LogLevel logLevel;
                std::string message;

                LogMessage(Logger::LogType logType, Logger::LogLevel logLevel, std::string message)
                    : logType(logType), logLevel(logLevel), message(message)
                {

                } 
        };
        
        static Logger* instance;
        BlockingQueue<Logger::LogMessage> priorityMessages;
        BlockingQueue<Logger::LogMessage> generalMessages;
        std::map<Logger::LogType, std::ofstream> fileStreams;

        void InitLogger();
        void InitLogFile(Logger::LogType logType);
        void WriteThread(bool isPriorityThread);
        std::string GetCurrentDateTime();
};

#endif