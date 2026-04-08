#pragma once
#include <iostream>
#include "ITransportable.h"
class CargoBox :
    public ITransportable
{
protected:
    double weight;
    bool isFragile;
public:
    CargoBox(double weight, bool isFragile);
    ~CargoBox() override = default;

    double getWeight() const override;
    bool getIsFragile() const;
    friend std::ostream& operator<< (std::ostream& os, const CargoBox& cb);
};