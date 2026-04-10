#include "Person.h"

Person::Person(std::string name, double weight, int startFloor, int destinationFloor) : name(name), weight(weight), startFloor(startFloor), destinationFloor(destinationFloor)
{
}

double Person::getWeight() const
{
	return this->weight;
}

int Person::getDestination() const
{
	return this->destinationFloor;
}

std::string Person::getIcon() const
{
	return "👤";
}

std::string Person::getName() const
{
	return this->name;
}

int Person::getStartFloor() const
{
	return this->startFloor;
}

std::ostream& operator<<(std::ostream& os, const Person& p)
{
	os << "Pasager: " << p.name << " | Greutate: " << p.weight << " kg | Rol: " << p.getRole();
	return os;
}