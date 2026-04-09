#pragma once
#include "BaseRequest.h"
class ExternalRequest :
    public BaseRequest
{
protected:
    std::string direction;
public:
    ExternalRequest(int targetFloor, std::string direction);
    bool isUrgent() const override;
};