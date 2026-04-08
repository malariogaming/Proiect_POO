#pragma once
#include "Person.h"
class StandardPassenger :
    public Person
{
protected:

public:
    StandardPassenger(std::string name, double weight, int destinationFloor);
    ~StandardPassenger() override = default;

    std::string getRole() const override;
};