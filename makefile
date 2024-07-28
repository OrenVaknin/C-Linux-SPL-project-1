all: clean compile link

compile:
	g++ -g -Wall -Weffc++ -std=c++11 -c -o bin/BaseAction.o src/BaseAction.cpp -I include
	g++ -g -Wall -Weffc++ -std=c++11 -c -o bin/Customer.o src/Customer.cpp -I include
	g++ -g -Wall -Weffc++ -std=c++11 -c -o bin/Order.o src/Order.cpp -I include
	g++ -g -Wall -Weffc++ -std=c++11 -c -o bin/Volunteer.o src/Volunteer.cpp -I include
	g++ -g -Wall -Weffc++ -std=c++11 -c -o bin/WareHouse.o src/WareHouse.cpp -I include
	g++ -g -Wall -Weffc++ -std=c++11 -c -o bin/main.o src/main.cpp -I include
	g++ -g -Wall -Weffc++ -std=c++11 -c -o bin/DriverVolunteer.o src/DriverVolunteer.cpp -I include
	g++ -g -Wall -Weffc++ -std=c++11 -c -o bin/CollectorVolunteer.o src/CollectorVolunteer.cpp -I include
	g++ -g -Wall -Weffc++ -std=c++11 -c -o bin/LimitedDriverVolunteer.o src/LimitedDriverVolunteer.cpp -I include
	g++ -g -Wall -Weffc++ -std=c++11 -c -o bin/LimitedCollectorVolunteer.o src/LimitedCollectorVolunteer.cpp -I include

link:
	g++ -o bin/warehouse bin/*.o
# -g -Wall -Weffc++ -std=c++11

# Run the executable with command-line arguments
run: warehouse
	@echo 'Running target: warehouse'
	./bin/warehouse "/home/users/bsc/goldidan/Desktop/ps1/configFileExample.txt"
	@echo ' '

clean:
	rm -f bin/*.o bin/warehouse