#include "../include/Volunteer.h"

LimitedDriverVolunteer::LimitedDriverVolunteer(int id, const string& name, int maxDistance, int distancePerStep, int maxOrders)
	: DriverVolunteer(id, name, maxDistance, distancePerStep), maxOrders(maxOrders), ordersLeft(maxOrders) {
}

LimitedDriverVolunteer* LimitedDriverVolunteer::clone() const {
	return new LimitedDriverVolunteer(*this);
}

int LimitedDriverVolunteer::getMaxOrders() const {
	return maxOrders;
}

int LimitedDriverVolunteer::getNumOrdersLeft() const {
	return ordersLeft;
}

bool LimitedDriverVolunteer::hasOrdersLeft() const {
	return ordersLeft > 0;
}

bool LimitedDriverVolunteer::canTakeOrder(const Order& order) const {
	return hasOrdersLeft() && DriverVolunteer::canTakeOrder(order);
}

void LimitedDriverVolunteer::acceptOrder(const Order& order) {
	DriverVolunteer::acceptOrder(order);
	ordersLeft--;
}

string LimitedDriverVolunteer::toString() const {
	string s = Volunteer::toString();
	s += "Distance Left: " + (isBusy() ? to_string(getDistanceLeft()) + "\n" : "None\n");
	s += "Orders Left: " + to_string(ordersLeft) + "\n";
	return s;
}
