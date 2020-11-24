#ifndef CLIENT_SUBSCRIPTION_H
#define CLIENT_SUBSCRIPTION_H

#include <functional>
#include <memory>

#include "../response/response_base.hpp"

class ClientSubscription {
    public:
        enum class SubscriptionType {
            NONE,
            MARKET_DATA
        };

        SubscriptionType subscriptionType;
        std::function<void(std::unique_ptr<ResponseBase>)> callback;

        ClientSubscription() {
            this->subscriptionType = SubscriptionType::NONE;
        }
};

#endif