#include "MyException.h"

MyException::MyException(std::string descriere) : descriere(descriere)
{
}

std::string MyException::what() const
{
	return "Eroare Default: " + this->descriere;
}