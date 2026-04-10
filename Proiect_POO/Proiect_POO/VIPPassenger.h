#pragma once
#include "Person.h"
class VIPPassenger :
    public Person
{
protected:
    int priorityLevel;
public:
    VIPPassenger(std::string name, double weight, int startFloor, int destinationFloor, int priorityLevel);
    ~VIPPassenger() override = default;

    std::string getRole() const override;
    std::string getIcon() const override;
};