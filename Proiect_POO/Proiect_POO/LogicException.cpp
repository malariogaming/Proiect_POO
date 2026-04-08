#include "LogicException.h"

LogicException::LogicException(std::string descriere, int errorCode) : MyException(descriere), errorCode(errorCode)
{
}

std::string LogicException::what() const
{
	return "Eroare Logica [Cod " + std::to_string(this->errorCode) + "]: " + this->descriere;
}