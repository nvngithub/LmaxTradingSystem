#ifndef SERVICE_H
#define SERVICE_H

#include <memory>
#include <thread>
#include <functional>
#include <mutex>
#include <string>

#include "../models/subscription/client_subscription.hpp"
#include "../models/request/request_base.hpp"
#include "../models/request/authenticate/autheticate_request.hpp"
#include "../models/request/heart_beat_request/heart_beat_request.hpp"
#include "../models/response/response_base.hpp"
#include "../models/response/market_data_response/market_data_response.hpp"

#include "../blocking_queue/blocking_queue.hpp"
#include "../fix/fix_converter.hpp"
#include "../sock_comm/sock_comm.hpp"
#include "../client_handler/client_handler.hpp"
#include "../logger/logger.hpp"

class Service
{
    private: 
        const std::string TRADING_HOSTNAME { "fix-order.london-demo.lmax.com" };
        const std::string MARKET_DATA_HOSTNAME { "fix-marketdata.london-demo.lmax.com" };

        bool isMarketDataConnection = false;
        Logger::LogType logType;

    private:
        
        BlockingQueue<std::unique_ptr<RequestBase>> clientQueue; // requests received from client apps
        BlockingQueue<std::unique_ptr<ResponseBase>> serviceQueue; // response received from services
        BlockingQueue<std::unique_ptr<ResponseBase>> marketDataQueue;

        std::map<ClientSubscription::SubscriptionType, ClientSubscription> subcriptions;

        std::mutex clientQueueMutex;
        std::mutex serviceQueueMutex;
        std::mutex marketDataQueueMutex;

        std::thread socketReadThread;
        std::thread heartbeatThread;
        std::thread clientQueueHandlerThread;
        std::thread serviceQueueHandlerThread;
        std::thread marketDataQueueHandlerThread;

        std::unique_ptr<SocketCommunicator> serviceCommunicator = nullptr;
        
    private:
        void StartThreads();
        void SocketReader();
        void ClientQueueHandler();
        void ServiceQueueHandler();
        void MarketDataQueueHandler();
        void HeartBeatThread();

    public:
        void Start(bool isMarketDataConnection);
        void StartHeartBeat();
        void AddRequest(std::unique_ptr<RequestBase>&& request);
        void Subscribe(ClientSubscription&& subscription);
};

#endif
