#include "BaseElevator.h"

BaseElevator::BaseElevator(int id, double maxWeight, int minFloor, int maxFloor) : id(id), currentFloor(0), targetFloor(0), minFloor(minFloor), maxFloor(maxFloor), maxWeight(maxWeight), currentWeight(0.0), status(ElevatorStatus::IDLE)
{
}

void BaseElevator::moveTo(int floor)
{
	if (floor < minFloor || floor > maxFloor) {
		throw InvalidFloorException(floor);
	}

	if (floor == currentFloor) {
		std::cout << "[Lift " << id << "] este deja la etajul " << floor << std::endl;
		return;
	}

	this->targetFloor = floor;
	this->status = (targetFloor > currentFloor) ? ElevatorStatus::MOVING_UP : ElevatorStatus::MOVING_DOWN;

	std::string directie = (status == ElevatorStatus::MOVING_UP) ? "SUS" : "JOS";
	std::cout << "[Lift " << id << "] se deplaseaza " << directie << ": " << currentFloor << " -> " << targetFloor << std::endl;

	//deplasare...

	currentFloor = targetFloor;
	status = ElevatorStatus::IDLE;

	std::cout << "[Lift " << id << "] s-a oprit la etajul " << currentFloor << std::endl;
}

void BaseElevator::openDoors()
{
	status = ElevatorStatus::DOORS_OPEN;
	std::cout << "[Lift " << id << "] usile s-au DESCHIS la etajul " << currentFloor << std::endl;
}

void BaseElevator::closeDoors()
{
	status = ElevatorStatus::IDLE;
	std::cout << "[Lift " << id << "] Usile s-au INCHIS la etajul" << currentFloor << std::endl;
}

void BaseElevator::loadItem(std::shared_ptr<ITransportable> item)
{
	if (!canAcceptWeight(item->getWeight())) {
		double excess = (currentWeight + item->getWeight()) - maxWeight;
		throw OverweightException(id, excess);
	}

	cargo.push_back(item);
	currentWeight += item->getWeight();

	std::cout << "[Lift " << id << "] incarcat cu: " << item->getWeight() << " kg. Greutate totala actuala: " << currentWeight << "/" << maxWeight << " kg" << std::endl;
}

void BaseElevator::unloadItem(std::shared_ptr<ITransportable> item)
{
	auto it = std::find(cargo.begin(), cargo.end(), item);

	if (it != cargo.end()) {
		currentWeight -= (*it)->getWeight();
		cargo.erase(it);
		std::cout << "[Lift " << id << "] descarcare reusita la etajul " << currentFloor << std::endl;
	}
	else {
		std::cout << "[Eroare] Obiectul nu a fost gasit in liftul " << id << std::endl;
	}
}

bool BaseElevator::canAcceptWeight(double weight) const
{
	return (this->currentWeight + weight) <= this->maxWeight;
}

int BaseElevator::getCurrentFloor() const
{
	return this->currentFloor;
}

int BaseElevator::getId() const
{
	return this->id;
}

ElevatorStatus BaseElevator::getStatus() const
{
	return this->status;
}

BaseElevator& BaseElevator::operator=(const BaseElevator& other)
{
	if (this != &other) {
		this->id = other.id;
		this->currentFloor = other.currentFloor;
		this->targetFloor = other.targetFloor;
		this->minFloor = other.minFloor;
		this->maxFloor = other.maxFloor;
		this->maxWeight = other.maxWeight;
		this->currentWeight = other.currentWeight;
		this->status = other.status;

		this->cargo = other.cargo; //shallow copy
	}
	return *this;
}