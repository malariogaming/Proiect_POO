#pragma once
#include "BaseElevator.h"
class FreightElevator :
    public BaseElevator
{
protected:

public:
    FreightElevator(int id, double maxWeight, int minFloor, int maxFloor);
    ~FreightElevator() override = default;

    std::string getType() const override;
    void enableHeavyMode();
    bool canAcceptWeight(double weight) const override;
};