#pragma once
#include "Person.h"
class StandardPassenger :
    public Person
{
protected:

public:
    StandardPassenger(std::string name, double weight, int startFloor, int destinationFloor);
    ~StandardPassenger() override = default;

    std::string getRole() const override;
    std::string getIcon() const override;
};