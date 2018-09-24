#include <iostream>
#include <chrono>
#include "Person.h"
#include "Simulation.h"
using namespace std;
using namespace std::chrono;

int main() {

	Simulation sim;
	sim.start(50, 500, 0.3f, 0.45f, 0.25f, 100);
	sim.run(50);
	
	cin.ignore();
}