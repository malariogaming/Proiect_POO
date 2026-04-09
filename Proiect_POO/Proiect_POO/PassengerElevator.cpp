#include "PassengerElevator.h"

PassengerElevator::PassengerElevator(int id, double maxWeight, int minFloor, int maxFloor, int maxPeople) : BaseElevator(id, maxWeight, minFloor, maxFloor), maxPeople(maxPeople)
{
}

std::string PassengerElevator::getType() const
{
	return "Passenger Elevator";
}

void PassengerElevator::playElevatorMusic()
{
    std::cout << "[Lift " << id << "] * Se aude o melodie relaxanta *" << std::endl;
}

void PassengerElevator::loadItem(std::shared_ptr<ITransportable> item)
{
    if (this->cargo.size() >= this->maxPeople) {
        std::cout << "[Lift " << id << "] EROARE: Capacitate maxima de persoane atinsa" << std::endl;
        return;
    }

    BaseElevator::loadItem(item);
}

bool PassengerElevator::canAcceptWeight(double weight) const
{
    bool hasWeightRoom = BaseElevator::canAcceptWeight(weight);
    bool hasPeopleRoom = cargo.size() < (size_t)maxPeople;
    return hasWeightRoom && hasPeopleRoom;
}