#pragma once
#include <iostream>
#include "ITransportable.h"
class CargoBox :
    public ITransportable
{
protected:
    double weight;
    bool isFragile;
    int startFloor;
    int destinationFloor;
public:
    CargoBox(double weight, bool isFragile, int startFloor, int destinationFloor);
    ~CargoBox() override = default;

    double getWeight() const override;
    int getDestination() const override;
    bool getIsFragile() const;
    friend std::ostream& operator<< (std::ostream& os, const CargoBox& cb);

    std::string getName() const override;
    int getStartFloor() const override;
};