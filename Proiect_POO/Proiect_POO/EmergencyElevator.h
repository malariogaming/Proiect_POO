#pragma once
#include "BaseElevator.h"
class EmergencyElevator :
    public BaseElevator
{
protected:
    bool isEmergencyActive;
public:
    EmergencyElevator(int id, double maxWeight, int minFloor, int maxFloor);
    ~EmergencyElevator() override = default;

    std::string getType() const override;
    void activateRedCode();
    bool canAcceptWeight(double weight) const override;
    void moveTo(int floor) override;
};