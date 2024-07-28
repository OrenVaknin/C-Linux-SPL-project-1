#include "../include/Volunteer.h"

Volunteer::Volunteer(int id, const string& name): 
     completedOrderId(NO_ORDER), activeOrderId(NO_ORDER), id(id), name(name) {}

int Volunteer::getId() const {
    return id;
}

const string& Volunteer::getName() const
{
    return name;
}

int Volunteer::getActiveOrderId() const
{
    return activeOrderId;
}

int Volunteer::getCompletedOrderId() const
{
    return completedOrderId;
}

bool Volunteer::isBusy() const
{
    return activeOrderId != NO_ORDER;
}

string Volunteer::toString() const //this is toString
{
	string s;
	s = "VolunteerID: " + to_string(getId()) + "\n";
    s += (isBusy()?  "Is Busy: True":  "Is Busy: False");
	s += "\nOrderID: " + (getActiveOrderId() != NO_ORDER ? to_string(getActiveOrderId()) : "None") + "\n";
	return s;
}