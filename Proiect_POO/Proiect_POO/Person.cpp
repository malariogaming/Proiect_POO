#include "Person.h"

Person::Person(std::string name, double weight, int destinationFloor) : name(name), weight(weight), destinationFloor(destinationFloor)
{
}

double Person::getWeight() const
{
	return this->weight;
}

int Person::getDestinationFloor() const
{
	return this->destinationFloor;
}

std::ostream& operator<<(std::ostream& os, const Person& p)
{
	os << "Pasager: " << p.name << " | Greutate: " << p.weight << " kg | Rol: " << p.getRole();
	return os;
}