#ifndef RESPONSE_BASE_H
#define RESPONSE_BASE_H

#include <string>

class ResponseBase {
    public:
        virtual void FromFix(std::string fix) = 0;
};

#endif