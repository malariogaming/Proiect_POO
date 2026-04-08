#include "ElevatorException.h"

ElevatorException::ElevatorException(std::string descriere, int elevatorId) : MyException(descriere), elevatorId(elevatorId)
{
}

std::string ElevatorException::what() const
{
	return "Eroare Lift [" + std::to_string(this->elevatorId) + "]: " + this->descriere;
}