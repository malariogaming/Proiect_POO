#include "CargoBox.h"

CargoBox::CargoBox(double weight, bool isFragile, int destinationFloor) : weight(weight), isFragile(isFragile), destinationFloor(destinationFloor)
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

std::ostream& operator<<(std::ostream& os, const CargoBox& cb)
{
	os << "Colet: " << cb.weight << " kg | Fragil: " << (cb.isFragile ? "DA" : "NU");
	return os;
}