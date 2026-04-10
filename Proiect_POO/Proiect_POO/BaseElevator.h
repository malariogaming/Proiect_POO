#pragma once
#include "IElevator.h"

enum class ElevatorStatus {
    IDLE,
    MOVING_UP,
    MOVING_DOWN,
    DOORS_OPEN,
    OUT_OF_SERVICE
};

class BaseElevator :
    public IElevator
{
protected:
    int id;
    int currentFloor;
    int targetFloor;
    int minFloor;
    int maxFloor;
    double maxWeight;
    double currentWeight;
    ElevatorStatus status;

    std::vector<std::shared_ptr<ITransportable>> cargo;
public:
    BaseElevator(int id, double maxWeight, int minFloor, int maxFloor);
    virtual ~BaseElevator() override = default;

    virtual void moveTo(int floor) override;
    void openDoors() override;
    void closeDoors() override;
    virtual void loadItem(std::shared_ptr<ITransportable> item) override;
    void unloadItem(std::shared_ptr<ITransportable> item) override;
    virtual bool canAcceptWeight(double weight) const;

    int getCurrentFloor() const override;
    int getId() const override;
    ElevatorStatus getStatus() const;
    void setStatus(ElevatorStatus status);
    BaseElevator& operator=(const BaseElevator& other);
    const std::vector<std::shared_ptr<ITransportable>>& getCargo() const;
    virtual std::string getType() const = 0;

    bool hasPassengersForFloor(int floor) const;
    void unloadPassengersAt(int floor);
    bool hasDestination(int floor) const;

    void setCurrentFloor(int floor);
    void clearCargo();

    double getMaxWeight();
};