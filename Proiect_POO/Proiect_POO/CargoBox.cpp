#include "CargoBox.h"

CargoBox::CargoBox(double weight, bool isFragile, int startFloor, int destinationFloor) : weight(weight), isFragile(isFragile), startFloor(startFloor), destinationFloor(destinationFloor)
{
}

double CargoBox::getWeight() const
{
	return this->weight;
}

int CargoBox::getDestination() const
{
	return this->destinationFloor;
}

bool CargoBox::getIsFragile() const
{
	return this->isFragile;
}

std::string CargoBox::getName() const
{
	return "CargoBox" + std::to_string(this->weight) + "KG";
}

int CargoBox::getStartFloor() const
{
	return this->startFloor;
}

std::ostream& operator<<(std::ostream& os, const CargoBox& cb)
{
	os << "Colet: " << cb.weight << " kg | Fragil: " << (cb.isFragile ? "DA" : "NU");
	return os;
}