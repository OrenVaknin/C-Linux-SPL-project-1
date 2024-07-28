#include "../include/BaseAction.h"

extern WareHouse* backup;

BaseAction::BaseAction() : errorMsg() , status(){}

ActionStatus BaseAction::getStatus() const
{
	return ActionStatus();
}

void BaseAction::complete()
{
	status = ActionStatus::COMPLETED;
}

void BaseAction::error(string errorMsg)
{
	status = ActionStatus::ERROR;
	this->errorMsg = errorMsg;
	cout << "Error: " + errorMsg;
}

string BaseAction::getErrorMsg() const
{
	return errorMsg + "\n";
}

string BaseAction::toString() const
{
	if (status == ActionStatus::COMPLETED)
		return "Completed\n";
	return "Error";
}
//-----------------------------------------------------------------


SimulateStep::SimulateStep(int numOfSteps) : numOfSteps(numOfSteps) {}

void SimulateStep::act(WareHouse& wareHouse)
{
	for (int i = 0; i < numOfSteps; i++)
		wareHouse.preformStep();
	complete();
}

string SimulateStep::toString() const
{
	return "simulateStep " + to_string(numOfSteps) + " " + BaseAction::toString()+"\n";
}

SimulateStep* SimulateStep::clone() const
{
	return new SimulateStep(*this);

}
//-----------------------------------------------------------------


AddOrder::AddOrder(int id) : customerId(id) {}

void AddOrder::act(WareHouse& wareHouse)
{
	if (customerId >= 0 && customerId < wareHouse.getCustomerCounter()) //check if the customer exists
	{
		Customer& cus = wareHouse.getCustomer(customerId);

		if (cus.getId()!=-1 && cus.canMakeOrder()) // check if he can order
		{
			int id = wareHouse.nextOrderId();
			int dis = cus.getCustomerDistance();
			cus.addOrder(id);
			wareHouse.addOrder(new Order(id, customerId, dis));
			complete();
		}
		else
			error("Cannot place this order!\n");
	}
	else
		error("Cannot place this order! - Customer doesn't exist\n");
}

string AddOrder::toString() const
{
	// order 4
	return "Order " + to_string(customerId) + " " + BaseAction::toString() + "\n";
}

AddOrder* AddOrder::clone() const
{
	return new AddOrder(*this);
}
//-----------------------------------------------------------------


AddCustomer::AddCustomer(const string& customerName, const string& customerType, int distance, int maxOrders) :
	customerName(customerName), customerType(SetType(customerType)), distance(distance), maxOrders(maxOrders) {}

CustomerType AddCustomer::SetType(const string& customerType)
{
	if (customerType == "soldier")
		return CustomerType::Soldier;
	return CustomerType::Civilian;
}

void AddCustomer::act(WareHouse& wareHouse)
{
	//wareHouse.addCustomer(customerType, customerName, distance, maxOrders);
	Customer* c;
	if (customerType == CustomerType::Soldier)
		c = new SoldierCustomer(wareHouse.nextCustomerId(), customerName, distance, maxOrders);
	else
		c = new CivilianCustomer(wareHouse.nextCustomerId(), customerName, distance, maxOrders);
	wareHouse.addCustomer(c);
	complete();
}


AddCustomer* AddCustomer::clone() const
{
	return new AddCustomer(*this);
}

string AddCustomer::toString() const
{
	//  customer <customer_name> <customer_type> <customer_distance> <max_orders>
	string s = "Customer " + customerName + " ";
	s += (customerType == CustomerType::Soldier ? " Soldier " : " Civilian ");
	s += to_string(distance) + " " + to_string(maxOrders) + " ";
	return s + BaseAction::toString() + "\n";
}
//-----------------------------------------------------------------



PrintOrderStatus::PrintOrderStatus(int id) : orderId(id) {}

void PrintOrderStatus::act(WareHouse& wareHouse)
{
	if (wareHouse.getOrderCounter() <= orderId || orderId < 0)
		error("Order doesn't exist\n");
	else
	{
		cout << wareHouse.getOrder(orderId).toString() << endl;
		complete();
	}
}

PrintOrderStatus* PrintOrderStatus::clone() const
{
	return new PrintOrderStatus(*this);
}

string PrintOrderStatus::toString() const
{
	// orderStatus <order_id>
	return "OrderStatus " + to_string(orderId)  + " " +  BaseAction::toString();
}
//-----------------------------------------------------------------



PrintCustomerStatus::PrintCustomerStatus(int customerId) : customerId(customerId) {}

void PrintCustomerStatus::act(WareHouse& wareHouse)
{
	if (customerId<0 || customerId>=wareHouse.getCustomerCounter()) //checking if he exists
		error("Customer doesn't exist\n");
	else{
		vector<Order*>* my_orders = new vector<Order*>();
		wareHouse.myOrders(customerId, my_orders);

			cout << "CustomerID: " + to_string(customerId) + "\n";
			for (Order* o : *my_orders)
				cout << o->toStringByStats();
			cout << "numOrdersLeft: " + to_string(wareHouse.getCustomer(customerId).getMaxOrders() - wareHouse.getCustomer(customerId).getNumOrders()) + "\n\n";
		
		delete my_orders;
		complete();
	}
}

PrintCustomerStatus* PrintCustomerStatus::clone() const
{
	return new PrintCustomerStatus(*this);
}

string PrintCustomerStatus::toString() const
{
	// customerStatus <customer_id>
	return "CustomerStatus " + to_string(customerId)  + " " +  BaseAction::toString();
}

//-----------------------------------------------------------------


PrintVolunteerStatus::PrintVolunteerStatus(int id) : volunteerId(id) {}

void PrintVolunteerStatus::act(WareHouse& wareHouse)
{
	Volunteer* v = &(wareHouse.getVolunteer(volunteerId));
	if (v->getId() != -1)
	{
		cout << v->toString();
		complete();
	}
	else
		error("Volunteer doesn't exist!\n");
}

PrintVolunteerStatus* PrintVolunteerStatus::clone() const
{
	return new PrintVolunteerStatus(*this);
}

string PrintVolunteerStatus::toString() const
{
	//  volunteerStatus <volunteer_id>
	return "VolunteerStatus " + to_string(volunteerId)  + " " +  BaseAction::toString();
}
//-----------------------------------------------------------------



PrintActionsLog::PrintActionsLog() {}

void PrintActionsLog::act(WareHouse& wareHouse)
{
	for (BaseAction* a : wareHouse.getActions())
		cout << a->toString() + "\n";
	complete();
}

PrintActionsLog* PrintActionsLog::clone() const
{
	return new PrintActionsLog(*this);
}

string PrintActionsLog::toString() const
{
	return "Log " + BaseAction::toString() +"\n";
}
//-----------------------------------------------------------------



Close::Close() {}

void Close::act(WareHouse& wareHouse)
{
	for (int i = 0; i < wareHouse.getOrderCounter(); i++)
		cout << wareHouse.getOrder(i).toStringByStatsNID();
	wareHouse.close();
	complete();
}

Close* Close::clone() const
{
	return new Close(*this);
}

string Close::toString() const
{
	return "Close " + BaseAction::toString() + "\n";
}
//------------------------------------------------------------------


BackupWareHouse::BackupWareHouse() {}

void BackupWareHouse::act(WareHouse& wareHouse)
{
	if (backup != nullptr)
		delete backup;
	backup = wareHouse.clone();
	complete();
}

BackupWareHouse* BackupWareHouse::clone() const
{
	return new BackupWareHouse(*this);
}

string BackupWareHouse::toString() const
{
	return "BackUp " + BaseAction::toString();
} 
//-----------------------------------------------------------------

RestoreWareHouse::RestoreWareHouse() {}

void RestoreWareHouse::act(WareHouse& wareHouse)
{
	if (backup != nullptr) {
		//wareHouse = *backup->clone();
		wareHouse = *backup;
		complete();
	}
	else
		error("No backup available");
}

RestoreWareHouse* RestoreWareHouse::clone() const
{
	return new RestoreWareHouse(*this);
}

string RestoreWareHouse::toString() const
{
	if (backup == nullptr) {
		return "No backup available";
	}
	return "Restore " + BaseAction::toString();
}