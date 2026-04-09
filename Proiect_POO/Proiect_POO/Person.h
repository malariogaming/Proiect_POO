#pragma once
#include <iostream>
#include <string>
#include "ITransportable.h"
class Person :
    public ITransportable
{
protected:
    std::string name;
    double weight;
    int destinationFloor;
public:
    Person(std::string name, double weight, int destinationFloor);
    virtual ~Person() override = default;

    double getWeight() const override;
    int getDestination() const override;
    virtual std::string getRole() const = 0;
    friend std::ostream& operator << (std::ostream& os, const Person& p);
};