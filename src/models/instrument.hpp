#ifndef INSTRUMENT_H
#define INSTRUMENT_H

#include <string>

class Instrument {
    public:
        std::string SecurityID;
        const std::string SecurityIDSource = "8";

        Instrument() = default;
        
        Instrument(const Instrument& instrument) = delete;
        
        Instrument(Instrument&& instrument) noexcept : Instrument() {
            this->SecurityID = std::move(instrument.SecurityID);
        }

        Instrument(std::string&& securityID) noexcept : Instrument() {
            this->SecurityID = std::move(securityID);
        }

        Instrument& operator=(const Instrument& instrument) = delete;
        
        Instrument& operator=(Instrument&& instrument) {
            this->SecurityID = std::move(instrument.SecurityID);
            return *this;
        }

        ~Instrument() = default;
};

#endif