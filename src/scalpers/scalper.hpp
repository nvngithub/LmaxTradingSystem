#ifndef SCALPER_H
#define SCALPER_H

#include <memory>

#include "../service/service.hpp"

class Scalper {
    public:
        virtual void Start() = 0;
        virtual void End() = 0;

        std::shared_ptr<Service> marketDataService;
        std::shared_ptr<Service> tradingService;
};

#endif