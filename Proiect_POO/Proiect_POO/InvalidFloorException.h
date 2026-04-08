#pragma once
#include "LogicException.h"
class InvalidFloorException :
    public LogicException
{
protected:
    int requestedFloor;
public:
    InvalidFloorException(int requestedFloor);
    ~InvalidFloorException() override = default;

    std::string what() const override;
};