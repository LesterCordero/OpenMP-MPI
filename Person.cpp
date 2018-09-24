#include"Person.h"
#include"Simulation.h"
Person::Person(int id) {
	debug_id = id;
	local_x = rand() % 10 + 1;
	local_y = rand() % 10 + 1;
}

void Person::updateMe() {

}

void Person::doMovement() {
	
	int xdir = 1;
	int ydir = 1;

	if ((rand() % 100) > 50) {
		xdir = -1;
	}

	if ((rand() % 100) > 50) {
		ydir = -1;
	}

	local_x += xdir;
	local_y += ydir;
}

void Person::doMatrix() {

}

int Person::getX() {
	return local_x;
}

int Person::getY() {
	return local_y;
}