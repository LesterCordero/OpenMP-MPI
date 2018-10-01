#include"Person.h"
#include"Simulation.h"

Person::Person(int id, int roomsize) {
	infected_ticks = 1;
	state_current = 0;
	debug_id = id;
	local_x = rand() % roomsize;
	local_y = rand() % roomsize;
}

bool Person::infectedForTooLong(int death_counter_max){
	if (infected_ticks > death_counter_max) {
		return true;
	}else {
		infected_ticks++;
		return false;
	}
}

int Person::getX() {
	return local_x;
}

int Person::getY() {
	return local_y;
}

int Person::getDebugID() {
	return debug_id;
}

int Person::getState() {
	return state_current;
}

void Person::setState(int state) {
	state_current = state;
}

void Person::setLocation(int x, int y, int roomsize) {
	if (x > roomsize - 1) {
		x = roomsize - 1 ;
	}

	if (y > roomsize - 1) {
		y = roomsize - 1;
	}

	if (x < 1) {
		x = 1;
	}

	if (y < 1) {
		y = 1;
	}

	local_x = x;
	local_y = y;
}