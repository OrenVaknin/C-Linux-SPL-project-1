#include "../include/Volunteer.h"

LimitedCollectorVolunteer::LimitedCollectorVolunteer(int id, const string& name, int coolDown, int maxOrders)
	: CollectorVolunteer(id, name, coolDown), maxOrders(maxOrders), ordersLeft(maxOrders) {
}

LimitedCollectorVolunteer* LimitedCollectorVolunteer::clone() const {
	return new LimitedCollectorVolunteer(*this);
}

bool LimitedCollectorVolunteer::hasOrdersLeft() const
{
	return ordersLeft > 0;
}

bool LimitedCollectorVolunteer::canTakeOrder(const Order& order) const
{
	return hasOrdersLeft() && CollectorVolunteer::canTakeOrder(order);
}

void LimitedCollectorVolunteer::acceptOrder(const Order& order) {
	CollectorVolunteer::acceptOrder(order);
	ordersLeft--;
}

int LimitedCollectorVolunteer::getMaxOrders() const {
	return maxOrders;
}

int LimitedCollectorVolunteer::getNumOrdersLeft() const {
	return ordersLeft;
}

string LimitedCollectorVolunteer::toString() const {
	string s = Volunteer::toString();
	s += "Time Left: " + (isBusy() ? to_string(getTimeLeft()) + "\n" : "None\n");
	s += "Orders Left: "+ to_string(ordersLeft) + "\n";
	return s;
}