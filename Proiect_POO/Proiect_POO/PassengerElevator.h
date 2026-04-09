#pragma once
#include "BaseElevator.h"
class PassengerElevator :
    public BaseElevator
{
protected:
    int maxPeople;
public:
    PassengerElevator(int id, double maxWeight, int minFloor, int maxFloor, int maxPeople);
    ~PassengerElevator() override = default;

    std::string getType() const override;
    void playElevatorMusic();
    void loadItem(std::shared_ptr<ITransportable> item) override;
    bool canAcceptWeight(double weight) const override;
};