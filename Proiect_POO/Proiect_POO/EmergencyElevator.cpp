#include "EmergencyElevator.h"

EmergencyElevator::EmergencyElevator(int id, double maxWeight, int minFloor, int maxFloor) : BaseElevator(id, maxWeight, minFloor, maxFloor), isEmergencyActive(false)
{
}

std::string EmergencyElevator::getType() const
{
	return "Emergency Elevator";
}

void EmergencyElevator::activateRedCode()
{
    this->isEmergencyActive = !(this->isEmergencyActive);
    if (this->isEmergencyActive) {
        status = ElevatorStatus::OUT_OF_SERVICE; // blocat pentru uzul normal
        std::cout << "[Lift " << id << "] !!! MOD URGENTA ACTIVAT !!!" << std::endl;
    }
    else {
        status = ElevatorStatus::IDLE;
        std::cout << "[Lift " << id << "] Mod urgenta dezactivat. Revine la operare normala" << std::endl;
    }
}

bool EmergencyElevator::canAcceptWeight(double weight) const
{
    return BaseElevator::canAcceptWeight(weight);
}

void EmergencyElevator::moveTo(int floor)
{
    if (isEmergencyActive) {
        std::cout << "[Lift " << id << "] Interventie rapida catre etajul " << floor << std::endl;
    }
    BaseElevator::moveTo(floor);
}