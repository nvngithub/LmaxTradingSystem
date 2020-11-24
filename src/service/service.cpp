#include "service.hpp"

void Service::Start(bool isMarketDataConnection)
{
    this->isMarketDataConnection = isMarketDataConnection;
    this->logType = (this->isMarketDataConnection ? Logger::LogType::GENERAL : Logger::LogType::GENERAL_TRADING);
    
    this->serviceCommunicator = std::make_unique<SocketCommunicator>(SocketCommunicator());
    this->serviceCommunicator->Init((this->isMarketDataConnection ? MARKET_DATA_HOSTNAME : TRADING_HOSTNAME), 443);

    this->StartThreads();
}

void Service::StartHeartBeat() {
    this->heartbeatThread = std::thread(std::bind(&Service::HeartBeatThread, this));
    this->heartbeatThread.detach();
}

void Service::StartThreads() {
    this->socketReadThread = std::thread(std::bind(&Service::SocketReader, this));
    this->socketReadThread.detach();

    this->clientQueueHandlerThread = std::thread(std::bind(&Service::ClientQueueHandler, this));
    this->clientQueueHandlerThread.detach();

    this->serviceQueueHandlerThread = std::thread(std::bind(&Service::ServiceQueueHandler, this));
    this->serviceQueueHandlerThread.detach();

    if(this->isMarketDataConnection) {
        this->marketDataQueueHandlerThread = std::thread(std::bind(&Service::MarketDataQueueHandler, this));
        this->marketDataQueueHandlerThread.detach();
    }
}

void Service::AddRequest(std::unique_ptr<RequestBase>&& request)
{
    Logger::Instance()->Add(this->logType, Logger::LogLevel::INFORMATION, "AddRequest::Adding new request");
    this->clientQueue.Push(std::move(request));
}

void Service::Subscribe(ClientSubscription&& subscription)
{
    this->subcriptions.emplace(std::make_pair(subscription.subscriptionType, std::move(subscription)));
}

void Service::SocketReader() {
    while(true) {
        if(this->serviceCommunicator != nullptr && this->serviceCommunicator->IsSocketConnected) {
            std::string fix = this->serviceCommunicator->RecvPacket();
            Logger::Instance()->Add(this->logType, Logger::LogLevel::INFORMATION, "Service response::" + fix);

            std::this_thread::sleep_for(std::chrono::milliseconds(50));
            
            std::unique_ptr<ResponseBase> response = FixConverter::ConvertFromFix<std::unique_ptr<ResponseBase>>(fix);

            if(dynamic_cast<MarketDataResponse*>(response.get()) && this->isMarketDataConnection) {
                this->marketDataQueue.Push(std::move(response));
            } else {
                this->serviceQueue.Push(std::move(response));
            }
        } else {
            Logger::Instance()->Add(this->logType, Logger::LogLevel::INFORMATION, "Service connection closed");
            break;
        }
    }

    Logger::Instance()->Add(this->logType, Logger::LogLevel::INFORMATION, "Exiting socket reader");
}

void Service::ClientQueueHandler() {
    while(true) {
        Logger::Instance()->Add(this->logType, Logger::LogLevel::INFORMATION, "Client queue handler waiting to sending.");
        std::string fix = FixConverter::ConvertToFix<std::unique_ptr<RequestBase>>(this->clientQueue.Pop());
        Logger::Instance()->Add(this->logType, Logger::LogLevel::INFORMATION, "Sending packet to service::" + fix);
        this->serviceCommunicator->SendPacket(fix.c_str());

        std::this_thread::sleep_for(std::chrono::seconds(1));
    }

    Logger::Instance()->Add(this->logType, Logger::LogLevel::INFORMATION, "Exiting client queue handler");
}

void Service::ServiceQueueHandler() {
    while(true) {
        if(this->subcriptions.count(ClientSubscription::SubscriptionType::NONE) > 0) {
            std::unique_ptr<ResponseBase> response = this->serviceQueue.Pop();
            (this->subcriptions[ClientSubscription::SubscriptionType::NONE]).callback(std::move(response));
        }
    }
}

void Service::MarketDataQueueHandler() {
    while(true) {
        if(this->subcriptions.count(ClientSubscription::SubscriptionType::MARKET_DATA) > 0) {
            std::unique_ptr<ResponseBase> response = this->marketDataQueue.Pop();
            (this->subcriptions[ClientSubscription::SubscriptionType::MARKET_DATA]).callback(std::move(response));
        }
    }
}

void Service::HeartBeatThread() {
    while(true) {
        std::this_thread::sleep_for(std::chrono::seconds(25));
    
        Logger::Instance()->Add(this->logType, Logger::LogLevel::INFORMATION, "HeartBeatThread::Adding heartbeat to queue");
        this->AddRequest(std::make_unique<HeartBeatRequest>(HeartBeatRequest()));
    }
}
