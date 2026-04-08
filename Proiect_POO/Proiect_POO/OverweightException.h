#pragma once
#include "ElevatorException.h"
class OverweightException :
    public ElevatorException
{
protected:
    double excessWeight;
public:
    OverweightException(int elevatorId, double excessWeight);
    ~OverweightException() override = default;

    std::string what() const override;
};