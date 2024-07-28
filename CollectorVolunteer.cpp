#include "../include/Volunteer.h"

CollectorVolunteer::CollectorVolunteer(int id, const string& name, int coolDown)
	: Volunteer(id, name), coolDown(coolDown), timeLeft(coolDown) {}


CollectorVolunteer* CollectorVolunteer::clone() const {
		return new CollectorVolunteer(*this);
}

void CollectorVolunteer::step() {
	if(decreaseCoolDown())
	{
		completedOrderId = activeOrderId;
		activeOrderId = NO_ORDER;
	}
}

int CollectorVolunteer::getCoolDown() const {
	return coolDown;
}

int CollectorVolunteer::getTimeLeft() const {
	return timeLeft;
}

bool CollectorVolunteer::decreaseCoolDown()
{
	timeLeft--;
	return timeLeft <= 0;
}

bool CollectorVolunteer::hasOrdersLeft() const {
	return true;
}

bool CollectorVolunteer::canTakeOrder(const Order& order) const {
	return !isBusy() && order.getStatus() == OrderStatus::PENDING;
}

void CollectorVolunteer::acceptOrder(const Order& order) {
	timeLeft = coolDown;
	activeOrderId = order.getId();
}

string CollectorVolunteer::toString() const {
	string s = Volunteer::toString();
	s += "Time Left: " + (isBusy()? to_string(getTimeLeft()) + "\n" : "None\n");
	s += "Orders Left: No Limit\n";
	return s; 
}