#include "../include/Order.h"

Order::Order(int id, int customerId, int distance) :
	id(id), customerId(customerId), distance(distance), status(OrderStatus::PENDING),
	collectorId(NO_VOLUNTEER), driverId(NO_VOLUNTEER) {}


Order* Order::clone()
{
    return new Order(*this);
}

int Order::getId() const {
	return id;
};
int Order::getCustomerId() const {
	return customerId;
};
void Order::setStatus(OrderStatus status) {
	this->status = status;
};
void Order::setCollectorId(int collectorId) {
	this->collectorId = collectorId;
};
void Order::setDriverId(int driverId) {
	this->driverId = driverId;
};
int Order::getCollectorId() const {
	return collectorId;
};
int Order::getDriverId() const {
	return driverId;
};
OrderStatus Order::getStatus() const {
	return status;
}
const string Order::toString() const
{
	string s = "\nOrderId: " + to_string(id) + "\n";
	s += "OrderStatus: ";
	switch (status) {
	case OrderStatus::PENDING:
		s += "Pending";
		break;
	case OrderStatus::COLLECTING:
		s += "Collecting";
		break;
	case OrderStatus::DELIVERING:
		s += "Delivering";
		break;
	case OrderStatus::COMPLETED:
		s += "Completed";
		break;
	}
	s += "\nCustomerId: " + to_string(customerId) + "\n";

	s += "Collector: " + ((status == OrderStatus::PENDING) ? "None" : to_string(collectorId));

	s += "\nDriver: " + ((status == OrderStatus::DELIVERING || status == OrderStatus::COMPLETED) ? to_string(driverId) : "None");

	return s;
}
int Order::getDistance() const
{
	return distance;
}

const string Order::toStringByStats() const //another toString with different syntax
{
	string s = "";
	switch (status) {
	case OrderStatus::PENDING:
		s += "Pending";
		break;
	case OrderStatus::COLLECTING:
		s += "Collecting";
		break;
	case OrderStatus::DELIVERING:
		s += "Delivering";
		break;
	case OrderStatus::COMPLETED:
		s += "Completed";
		break;
	}

	return "OrderID: " + to_string(id) + "\nOrderStatus: " + s + "\n";
}

const string Order::toStringByStatsNID() const //another toString with different syntax
{
	string s = "";
	switch (status) {
	case OrderStatus::PENDING:
		s += "Pending";
		break;
	case OrderStatus::COLLECTING:
		s += "Collecting";
		break;
	case OrderStatus::DELIVERING:
		s += "Delivering";
		break;
	case OrderStatus::COMPLETED:
		s += "Completed";
		break;
	}

	return "OrderID: " + to_string(id) + ", CustomerID: " + to_string(customerId) + ", OrderStatus: " + s + "\n";
}
Order& Order::getInvalidOrder()
{
	static Order invalidOrder(-1, -1 ,0);
	return invalidOrder;
}
