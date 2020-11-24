#include <iostream>
#include <memory>
#include <string>

#include "logger/logger.hpp"
#include "service/service.hpp"
#include "scalpers/scalper.hpp"
#include "scalpers/simple_scalper/simple_scalper.hpp"

int main(int argc, char** argv)
{
    Logger::Instance()->LoggerLevel = Logger::LogLevel::INFORMATION;
    
    /*Logger::Instance()->Add(Logger::LogType::GENERAL, Logger::LogLevel::INFORMATION, "Starting market data service...");
    std::shared_ptr<Service> marketDataService = std::shared_ptr<Service>(new Service());
    marketDataService->Start(true);*/

    Logger::Instance()->Add(Logger::LogType::GENERAL_TRADING, Logger::LogLevel::INFORMATION, "Starting trading service...");
    std::shared_ptr<Service> tradingService = std::shared_ptr<Service>(new Service());
    tradingService->Start(false);

    Logger::Instance()->Add(Logger::LogType::GENERAL, Logger::LogLevel::INFORMATION, "Starting simple scalper...");
    std ::unique_ptr<Scalper> scalper = std::make_unique<SimpleScalper>(SimpleScalper());
    //scalper->marketDataService = marketDataService;
    scalper->tradingService = tradingService;
    scalper->Start();

    std::string tmp;
    std::cin >> tmp;
}
