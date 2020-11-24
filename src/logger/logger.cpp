#include "logger.hpp"

Logger* Logger::instance = nullptr;

Logger* Logger::Instance()
{
    if(Logger::instance == nullptr)
    {
        Logger::instance = new Logger();

        Logger::instance->InitLogger();
    }

    return Logger::instance;
}

void Logger::Add(Logger::LogType logType, Logger::LogLevel logLevel, std::string message)
{
    if(static_cast<Logger::LogLevel>(logLevel) > static_cast<Logger::LogLevel>(this->LoggerLevel)) 
        return;

    switch (logType)
    {
        case Logger::LogType::GENERAL:
        case Logger::LogType::GENERAL_TRADING:
        case Logger::LogType::MARKET_DATA:
            this->generalMessages.Push(Logger::LogMessage { logType, logLevel, message } );
            break;
    
        case Logger::LogType::ORDER:
            this->priorityMessages.Push(Logger::LogMessage { logType, logLevel, message } );
            break;

        default:
            break;
    }
}

void Logger::InitLogger() 
{
    // create file streams
    
    InitLogFile(Logger::LogType::GENERAL);
    InitLogFile(Logger::LogType::GENERAL_TRADING);
    InitLogFile(Logger::LogType::MARKET_DATA);
    InitLogFile(Logger::LogType::ORDER);

    // create threads
    std::thread([this] { this->WriteThread(true); }).detach();
    std::thread([this] { this->WriteThread(false); }).detach();
}

void Logger::InitLogFile(Logger::LogType logType)
{
    std::string filePrefix = "general";
    switch (logType)
    {
        case Logger::LogType::GENERAL: filePrefix = "general"; break;
        case Logger::LogType::GENERAL_TRADING: filePrefix = "general_trading"; break;
        case Logger::LogType::MARKET_DATA: filePrefix = "market_data"; break;
        case Logger::LogType::ORDER: filePrefix = "orders"; break;
        default: break;
    }

    std::string logFilename { (boost::format("%s.%s") % filePrefix % "txt").str() };
    if(this->fileStreams.count(logType))
    {
        auto value = this->fileStreams.find(logType);
        if(value != this->fileStreams.end())
        {
            value->second.close();
            this->fileStreams.erase(value);
        }
    }
        
    this->fileStreams.emplace(make_pair(logType, std::ofstream(logFilename)));
}

void Logger::WriteThread(bool isPriorityThread)
{
    while(true)
    {
        Logger::LogMessage message = (isPriorityThread ? this->priorityMessages.Pop() : this->generalMessages.Pop());
        std::map<Logger::LogType, std::ofstream>::iterator value;
        std::string logMessageLevel {};
        switch (message.logType)
        {
            case Logger::LogType::GENERAL:
                value = this->fileStreams.find(Logger::LogType::GENERAL);
                logMessageLevel = "GENERAL";
                break;
            case Logger::LogType::GENERAL_TRADING:
                value = this->fileStreams.find(Logger::LogType::GENERAL_TRADING);
                logMessageLevel = "GENERAL_TRADING";
                break;
            case Logger::LogType::MARKET_DATA:
                value = this->fileStreams.find(Logger::LogType::MARKET_DATA);
                logMessageLevel = "MARKET_DATA";
                break;
            case Logger::LogType::ORDER:
                value = this->fileStreams.find(Logger::LogType::ORDER);
                logMessageLevel = "ORDER";
                break;
            default:
                value = this->fileStreams.end();
                break;
        }

        if(value != this->fileStreams.end())
        {
            value->second << this->GetCurrentDateTime();
            value->second << "::";
            value->second << logMessageLevel;
            value->second << "::";
            value->second << message.message;
            value->second << std::endl;
            value->second.flush();
        }
    }
}

std::string Logger::GetCurrentDateTime()
{
    auto now = std::chrono::system_clock::now();
    auto in_time_t = std::chrono::system_clock::to_time_t(now);

    std::stringstream ss;
    ss << std::put_time(std::localtime(&in_time_t), "%Y-%m-%d ");
    
    auto ms = std::chrono::duration_cast<std::chrono::milliseconds>(now.time_since_epoch()) % 1000;
    auto timer = std::chrono::system_clock::to_time_t(now);

    std::tm bt = *std::localtime(&timer);

    ss << std::put_time(&bt, "%H:%M:%S"); // HH:MM:SS
    ss << '.' << std::setfill('0') << std::setw(3) << ms.count();

    return ss.str();
}