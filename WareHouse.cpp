#include "../include/WareHouse.h"
#include "../include/BaseAction.h" 


WareHouse::WareHouse(const string& configFilePath) : isOpen(false) , actionsLog(), volunteers(), pendingOrders(),
inProcessOrders(), completedOrders(), customers(), customerCounter(0), volunteerCounter(0), orderCounter(0)
{
	ifstream configFile(configFilePath);

	if (!configFile.is_open()) {
		cerr << "Error: Unable to open the configuration file." << endl;
		// Handle the error accordingly
		return;
	}

	string line;
	while (getline(configFile, line))
	{
		// Ignore lines starting with '#'
		if (line.empty() || line[0] == '#')
			continue;


		// Remove comments at the end of the line
		line = line.substr(0, line.find('#'));

		istringstream iss(line);
		string type;
		iss >> type;

		if (type == "customer")
		{
			// # Customers - customer <customer_name> <customer_type> <customer_distance> <max_orders>
			string name, customerType;
			int distance, maxOrders;
			iss >> name >> customerType >> distance >> maxOrders;
			// Create Customer object and add it to your Warehouse
			if (customerType == "civilian")
				customers.push_back(new CivilianCustomer(nextCustomerId(), name, distance, maxOrders));
			else
				customers.push_back(new SoldierCustomer(nextCustomerId(), name, distance, maxOrders));
		}

		else if (type == "volunteer")
		{

			//# Volunteers - volunteer <volunteer_name> <volunteer_role> <volunteer_coolDown>/<volunteer_maxDistance> <distance_per_step>(for drivers only) <volunteer_maxOrders>(optional)
			string name, role;
			int cooldown, maxDistance, distancePerStep, maxOrders;
			iss >> name >> role;


			if (role == "collector" || role == "limited_collector")
			{
				iss >> cooldown >> maxOrders;

				if (iss.fail()) // no maxOrders has been given
					volunteers.push_back(new CollectorVolunteer(nextVolunteerId(), name, cooldown));
				else
					volunteers.push_back(new LimitedCollectorVolunteer(nextVolunteerId(), name, cooldown, maxOrders));
			}

			else if (role == "driver" || role == "limited_driver")
			{
				iss >> maxDistance >> distancePerStep >> maxOrders;

				if (iss.fail()) // no maxOrders has been given
					volunteers.push_back(new DriverVolunteer(nextVolunteerId(), name, maxDistance, distancePerStep));
				else
					volunteers.push_back(new LimitedDriverVolunteer(nextVolunteerId(), name, maxDistance, distancePerStep, maxOrders));
			}
		}
		else
			continue; // ERROR
	}

	configFile.close();
}

WareHouse::WareHouse(const WareHouse& other)
: isOpen(other.isOpen),
      actionsLog(),
      volunteers(),
      pendingOrders(),
      inProcessOrders(),
      completedOrders(),
      customers(),
      customerCounter(other.customerCounter),
      volunteerCounter(other.volunteerCounter),
      orderCounter(other.orderCounter)
{
	for (BaseAction* a : other.actionsLog)
		this->actionsLog.push_back(a->clone());

	for (Volunteer* v : other.volunteers)
		this->volunteers.push_back(v->clone());

	for (Order* o : other.pendingOrders)
		this->pendingOrders.push_back(o->clone());
	
	for (Order* o : other.inProcessOrders)
		this->inProcessOrders.push_back(o->clone());

	for (Order* o : other.completedOrders)
		this->completedOrders.push_back(o->clone());

	for (Customer* c : other.customers)
		this->customers.push_back(c->clone());
}


WareHouse::WareHouse(WareHouse&& other)
    : isOpen(other.isOpen),
      actionsLog(std::move(other.actionsLog)),
      volunteers(std::move(other.volunteers)),
      pendingOrders(std::move(other.pendingOrders)),
      inProcessOrders(std::move(other.inProcessOrders)),
      completedOrders(std::move(other.completedOrders)),
      customers(std::move(other.customers)),
      customerCounter(other.customerCounter),
      volunteerCounter(other.volunteerCounter),
      orderCounter(other.orderCounter) 
	  {
		other.isOpen = false; 
		other.customerCounter = 0;
		other.volunteerCounter = 0;
		other.orderCounter = 0;	
	  }

WareHouse& WareHouse::operator=(const WareHouse& other) {
	//This is the = operator, preforming deep copy
	this->clearMemory();
	
    if (this != &other) {
    
        isOpen = other.isOpen;
        customerCounter = other.customerCounter;
        volunteerCounter = other.volunteerCounter;
        orderCounter = other.orderCounter;

    
        for (const auto& action : other.actionsLog)
            actionsLog.push_back(action->clone()); 
        

        for (const auto& volunteer : other.volunteers) 
            volunteers.push_back(volunteer->clone());
    
        
        for (const auto& order : other.pendingOrders) 
            pendingOrders.push_back(order->clone());
        

        for (const auto& order : other.inProcessOrders) 
            inProcessOrders.push_back(order->clone());
        

        for (const auto& order : other.completedOrders) 
            completedOrders.push_back(order->clone());
        

        for (const auto& customer : other.customers) 
            customers.push_back(customer->clone());
        
    }
    return *this;
}

WareHouse::~WareHouse() {
    clearMemory();
}


WareHouse* WareHouse::clone()
{
	return new WareHouse(*this);
}

void WareHouse::start() {
	open();
	while (isOpen)
		handleInputAction();
		//test();
}
const vector<BaseAction*>& WareHouse::getActions() const
{
	return actionsLog;
}

void WareHouse::addOrder(Order* order)
{
	pendingOrders.push_back(order);
}

void WareHouse::addAction(BaseAction* action)
{
	actionsLog.push_back(action);
}

Customer& WareHouse::getCustomer(int customerId) const
{
	for (Customer* c : customers)
		if (c->getId() == customerId)
			return *c;

	//error
	return SoldierCustomer::getInvalidCustomer();
}

Volunteer& WareHouse::getVolunteer(int volunteerId) const
{
	for (Volunteer* v : volunteers)
		if (v->getId() == volunteerId)
			return *v;

	//error
	return DriverVolunteer::getInvalidVolunteer();
}
Order& WareHouse::getOrder(int orderId) const {

	Order* order = nullptr;

	for (Order* o : pendingOrders)
		if (o->getId() == orderId)
		{
			order = o;
			break;
		}
	if (order == nullptr)
		for (Order* o : inProcessOrders)
			if (o->getId() == orderId)
			{
				order = o;
				break;
			}

	if (order == nullptr)
		for (Order* o : completedOrders)
			if (o->getId() == orderId)
			{
				order = o;
				break;
			}

	if (order != nullptr)
		return *order;

	else
		// error
		return Order::getInvalidOrder();;
}

void WareHouse::close() {
	isOpen = false;
	
	clearMemory();
}
void WareHouse::open() {
	isOpen = true;
	cout << "Warehouse is open!" << endl;
}

void WareHouse::preformStep()
{
	bool freeCollector = true, freeDriver = true;
	// part 1
	for (auto it = pendingOrders.begin(); it != pendingOrders.end(); )
	{
		// Iterates pendingOrders
		Order* o = *it;
		bool toProg = true; //if an order was removed we shouldn't progress the iterator

		switch (o->getStatus())
		{
		case OrderStatus::PENDING: // Looking for collector if it's pending
			if (freeCollector)
			{
				Volunteer* v = findFreeVolunteer(*o);
				if (v != nullptr)
				{
					//connecting the order with the volunteer
					o->setCollectorId(v->getId());
					v->acceptOrder(*o);
					o->setStatus(OrderStatus::COLLECTING);
					switchOrdersVector(o->getId(), pendingOrders, inProcessOrders);
					toProg = false;
				}
				else
					freeCollector = false;
			}
			break;

		case OrderStatus::COLLECTING: // If it's in collecting we are looking for a driver
			if (freeDriver)
			{
				Volunteer* v = findFreeVolunteer(*o);
				if (v != nullptr)
				{
					//connecting the order with the volunteer
					o->setDriverId(v->getId());
					v->acceptOrder(*o);
					o->setStatus(OrderStatus::DELIVERING);
					switchOrdersVector(o->getId(), pendingOrders, inProcessOrders);
					toProg = false;
				}
				else
					freeDriver = false;
			}
			break;

		default:
			// COMPLETED or DELIVERING
			// ERROR
			// TODO
			break;
		}

		if(toProg)
			it++;
	}
	
	// parts 2 - 4
	for (auto it = volunteers.begin(); it != volunteers.end(); )
	{
		// Iterates volunteers
		Volunteer* v = *it;
		bool toProg = true;

		v->step(); // part 2 preforming step
		if (v->getActiveOrderId() == NO_ORDER) // part 3 if the volenteer is done, the order passes
		{
			Order o = getOrder(v->getCompletedOrderId());
			if (o.getStatus() == OrderStatus::COLLECTING){
				switchOrdersVector(o.getId(), inProcessOrders, pendingOrders);
			}
				
			else if (o.getStatus() == OrderStatus::DELIVERING)
			{
				switchOrdersVector(o.getId(), inProcessOrders, completedOrders);
				for (Order* op :completedOrders)
					if(op->getId() == o.getId())
					{
						op->setStatus(OrderStatus::COMPLETED);
						break;
					}	
				
			}
		}

		if (!v->hasOrdersLeft() && !v->isBusy()) // part 4  remiving limited volunteers
		{
			auto it = std::find(volunteers.begin(), volunteers.end(), v);
			if (it != volunteers.end())
			{
				volunteers.erase(it);
				toProg = false;
			}
				
			delete v;
		}

		if(toProg)
			it++;
	}
}

int WareHouse::nextCustomerId()
{
	return customerCounter++;
}

int WareHouse::nextVolunteerId()
{
	return volunteerCounter++;
}

int WareHouse::nextOrderId()
{
	return orderCounter++;
}

int WareHouse::getCustomerCounter()
{
	return customerCounter;
}

int WareHouse::getOrderCounter()
{
	return orderCounter;
}

int WareHouse::getVolunteerCounter()
{
	return volunteerCounter;
}

void WareHouse::addVolunteer(Volunteer* volunteer)
{
	volunteers.push_back(volunteer);
}

void WareHouse::addCustomer(Customer* customer)
{
	customers.push_back(customer);
}

void WareHouse::myOrders(int customerId, vector<Order*>* my_orders) // put all the customer orders in the vector
{
	//vector<Order*> my_orders = vector<Order*>();
	for(Order* o : pendingOrders)
		if (o->getCustomerId() == customerId)
			my_orders->push_back(o);

	for (Order* o : inProcessOrders)
		if (o->getCustomerId() == customerId)
			my_orders->push_back(o);

	for (Order* o : completedOrders)
		if (o->getCustomerId() == customerId)
			my_orders->push_back(o);

	//return my_orders;
}

Volunteer* WareHouse::findFreeVolunteer(const Order& order)
{
	for (Volunteer* v : volunteers)
		if (v->canTakeOrder(order))
			return v;
	return nullptr;
}


void WareHouse::switchOrdersVector(int order_id, vector<Order*>& source, vector<Order*>& destination)
{
    auto it = find_if(source.begin(), source.end(), [order_id](Order* o) {
        return o->getId() == order_id;
    });

    if (it != source.end())
    {
        Order* p = *it;
        source.erase(it);
        destination.push_back(p);
    }
}

void WareHouse::clearMemory() // clear the dynamic memory of the warehouse
{
	for (BaseAction* action : actionsLog) 
		delete action;
	actionsLog.clear();

	for (Volunteer* volunteer : volunteers)
		delete volunteer;
	volunteers.clear();

	for (Order* order : pendingOrders)
		delete order;
	pendingOrders.clear();

	for (Order* order : inProcessOrders)
		delete order;
	inProcessOrders.clear();

	for (Order* order : completedOrders)
		delete order;
	completedOrders.clear();

	for (Customer* customer : customers)
		delete customer;
	customers.clear();
}

void WareHouse::handleInputAction()
{
	string line;
	getline(cin, line);
	cout << endl;
	string actionType;
	istringstream iss(line);
	iss >> actionType;
	BaseAction* action;
	int userInputI1, userInputI2;

	if (actionType == "step"){
		if (iss >> userInputI1) // numOfSteps
			action = new SimulateStep(userInputI1);
	}
		

	else if (actionType == "order") {
		if (iss >> userInputI1) // customerId
			action = new AddOrder(userInputI1);
	}

	else if (actionType == "customer") {
		string userInputS1, userInputS2;

		// customerName, customerType, distance, maxOrders
		if (iss >> userInputS1 >> userInputS2 >> userInputI1 >> userInputI2 && (userInputS2 == "civilian" || userInputS2 == "soldier") && userInputI1>=0, userInputI2>=0)
			action = new AddCustomer(userInputS1, userInputS2, userInputI1, userInputI2);
	}

	else if (actionType == "orderStatus"){
		if (iss >> userInputI1) // orderId
			action = new PrintOrderStatus(userInputI1);
	}		

	else if (actionType == "customerStatus"){
		if (iss >> userInputI1) // customerID
			action = new PrintCustomerStatus(userInputI1);
	}
		
	else if (actionType == "volunteerStatus"){
		if (iss >> userInputI1) // volunteerID
			action = new PrintVolunteerStatus(userInputI1);
	}

	else if (actionType == "log") 
		action = new PrintActionsLog();

	else if (actionType == "close")
		action = new Close();

	else if (actionType == "backup")
		action = new BackupWareHouse();

	else if (actionType == "restore")
		action = new RestoreWareHouse();

	else{
		cout << "you have typed a wrong input, sir.\n";
		return;
	}
	action->act(*this);

	if(actionType != "close")
		addAction(action);
	else
		delete action;
	cout << "\n";
}

void WareHouse::test()
{
	string RED = "\033[31m";
	string BLUE = "\033[34m";
	string RESET = "\033[0m";

	cout << RESET <<"Enter actions path:" << endl;
	string line;
	getline(cin, line);
	cout << endl;

	ifstream configFile(line);

	if (!configFile.is_open()) {
		cerr << "Error: Unable to open the file." << endl;
		return;
	}

	
	while (getline(configFile, line))
	{
		
		if (line.empty() || line[0] == '#')
			continue;
		line = line.substr(0, line.find('#'));
		cout << RED << line << endl << endl << BLUE;

		istringstream iss(line);
		string actionType;
		iss >> actionType;


		BaseAction* action;
		int userInputI1, userInputI2;

		if (actionType == "step"){
			if (iss >> userInputI1) // numOfSteps
				action = new SimulateStep(userInputI1);
		}
			

		else if (actionType == "order") {
			if (iss >> userInputI1) // customerId
				action = new AddOrder(userInputI1);
		}

		else if (actionType == "customer") {
			string userInputS1, userInputS2;

			// customerName, customerType, distance, maxOrders
			if (iss >> userInputS1 >> userInputS2 >> userInputI1 >> userInputI2 && (userInputS2 == "civilian" || userInputS2 == "soldier") && userInputI1>=0, userInputI2>=0)
				action = new AddCustomer(userInputS1, userInputS2, userInputI1, userInputI2);
		}

		else if (actionType == "orderStatus"){
			if (iss >> userInputI1) // orderId
				action = new PrintOrderStatus(userInputI1);
		}		

		else if (actionType == "customerStatus"){
			if (iss >> userInputI1) // customerID
				action = new PrintCustomerStatus(userInputI1);
		}
			
		else if (actionType == "volunteerStatus"){
			if (iss >> userInputI1) // volunteerID
				action = new PrintVolunteerStatus(userInputI1);
		}

		else if (actionType == "log") 
			action = new PrintActionsLog();

		else if (actionType == "close")
			action = new Close();

		else if (actionType == "backup")
			action = new BackupWareHouse();

		else if (actionType == "restore")
			action = new RestoreWareHouse();

		else{
			cout << "wtf bro?\n";
			return;
		}
		
		if(actionType != "close")
		{
			action->act(*this);
			addAction(action);
		}
		else
		{
			action->act(*this);
			delete action;
		}
		cout << "\n";
	}
}

