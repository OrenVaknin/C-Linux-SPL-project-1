#include "../include/Volunteer.h"


DriverVolunteer::DriverVolunteer(int id, const string& name, int maxDistance, int distancePerStep)
	: Volunteer(id, name), maxDistance(maxDistance), distancePerStep(distancePerStep), distanceLeft() {}


DriverVolunteer* DriverVolunteer::clone() const
{
	return new DriverVolunteer(*this);
}

int DriverVolunteer::getDistanceLeft() const
{
	return distanceLeft;
}

int DriverVolunteer::getMaxDistance() const
{
	return maxDistance;
}

int DriverVolunteer::getDistancePerStep() const
{
	return distancePerStep;
}

bool DriverVolunteer::decreaseDistanceLeft()
{
	distanceLeft -= distancePerStep;
	return distanceLeft <= 0;
}

bool DriverVolunteer::hasOrdersLeft() const
{
	return true;
}

bool DriverVolunteer::canTakeOrder(const Order& order) const
{
	if (!isBusy() && order.getStatus() == OrderStatus::COLLECTING)
		if (order.getDistance() <= maxDistance)
			return true;
	return false;
}

void DriverVolunteer::acceptOrder(const Order& order)
{
	distanceLeft = order.getDistance();
	activeOrderId = order.getId();
}

void DriverVolunteer::step()
{
	if (decreaseDistanceLeft())
	{
		completedOrderId = activeOrderId;
		activeOrderId = NO_ORDER;
	}
}

string DriverVolunteer::toString() const
{
	string s = Volunteer::toString();
	s += "Distance Left: " + (isBusy() ? to_string(getDistanceLeft()) + "\n" : "None\n");
	s += "Orders Left: No Limit\n";
	return s;
}

Volunteer& DriverVolunteer::getInvalidVolunteer()
{
	static DriverVolunteer invalidVolunteer(-1, "", 0, 0);
	return invalidVolunteer;
}
