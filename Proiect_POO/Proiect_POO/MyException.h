#pragma once
#include <string>

class MyException
{
protected:
	std::string descriere;
public:
	MyException(std::string descriere);
	virtual ~MyException() = default;

	virtual std::string what() const;
};