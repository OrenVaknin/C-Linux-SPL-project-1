#include "../include/Customer.h"

Customer::Customer(int id, const string& name, int locationDistance, int maxOrders) :
	id(id), name(name), locationDistance(locationDistance), maxOrders(maxOrders), ordersId() {}

int Customer::getId() const 
{
	return id;
};

const string& Customer::getName() const
{
	return name;
}

int Customer::getCustomerDistance() const
{
	return locationDistance;
};

int Customer::getMaxOrders() const
{
	return maxOrders;
};

int Customer::getNumOrders() const
{
	return ordersId.size();
};

bool Customer::canMakeOrder() const
{
	return getMaxOrders() > getNumOrders();
}

const vector<int>& Customer::getOrders() const
{
	return ordersId;
}

int Customer::addOrder(int orderID)
{
	if (canMakeOrder())
	{
		ordersId.push_back(orderID);
		return orderID;
	}
	return -1;
}

SoldierCustomer::SoldierCustomer(int id, const string& name, int locationDistance, int maxOrders) :
	Customer(id, name, locationDistance, maxOrders) {}

SoldierCustomer* SoldierCustomer::clone() const
{
	return new SoldierCustomer(*this);
}

Customer& SoldierCustomer::getInvalidCustomer()
{
	static SoldierCustomer invalidCustomer(-1, "", 0 ,0);
	return invalidCustomer;
}



CivilianCustomer::CivilianCustomer(int id, const string& name, int locationDistance, int maxOrders)
	: Customer(id, name, locationDistance, maxOrders) {}

CivilianCustomer* CivilianCustomer::clone() const
{
	return new CivilianCustomer(*this);
}
