#include "CargoBox.h"

CargoBox::CargoBox(double weight, bool isFragile) : weight(weight), isFragile(isFragile)
{
}

double CargoBox::getWeight() const
{
	return this->weight;
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