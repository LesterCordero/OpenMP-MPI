#include"Person.h"
#include"Simulation.h"
Person::Person(int id) {
	debug_id = id;
	local_x = rand() % 10 + 1;
	local_y = rand() % 10 + 1;
}

void Person::updateMe(int** people_num_infected, int** people_num_recovered) {

	if (state_current == state_healthy) {
	
		if (people_num_infected[local_x][local_y] > 0) {
			for (int n = 0; n < people_num_infected[local_x][local_y]; n++) {
				if (rand() % 100 >  Simulation::infect_chance_pertick) {

					state_current = state_infected;
					people_num_infected[local_x][local_y] += 1;
					Simulation::stats_infected++;
				
				}
			}
		}

	}

	

}

void Person::doMovement(int** people_num_infected, int** people_num_recovered) {
	
	int xdir = 1;
	int ydir = 1;

	if ((rand() % 100) > 50) {
		xdir = -1;
	}

	if ((rand() % 100) > 50) {
		ydir = -1;
	}

	if (state_current == state_infected) {
		people_num_infected[local_x][local_y] -= 1;
		local_x += xdir;
		local_y += ydir;
		people_num_infected[local_x][local_y] += 1;
	}else {
		local_x += xdir;
		local_y += ydir;
	}

}

int Person::getX() {
	return local_x;
}

int Person::getY() {
	return local_y;
}