#include "ExternalRequest.h"

ExternalRequest::ExternalRequest(int targetFloor, std::string direction) : BaseRequest(targetFloor), direction(direction)
{
}

bool ExternalRequest::isUrgent() const
{
	return false; //de regula fals, fiindca cererile din interior au prioritate mai mare decat cele din exterior
}