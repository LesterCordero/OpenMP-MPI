#include <iostream>
#include <chrono>
#include "Person.h"
#include "Simulation.h"
#include<omp.h>

int cores = omp_get_num_procs();

using namespace std;
using namespace std::chrono;
steady_clock::time_point inicio;
steady_clock::time_point fin;
string tiempo;


void tiempo_inicio() {
	inicio = steady_clock::now();
}

void tiempo_final() {
	fin = steady_clock::now();
	duration<double> duracion = duration_cast<duration<double>>(fin - inicio);
	tiempo = to_string(duracion.count());
}


int main() {

	srand(time(NULL));

	Simulation sim;
	sim.start(5, 500, 0.3f, 0.45f, 0.25f, 100, cores);

	tiempo_inicio();
	sim.run(25);
	tiempo_final();
	sim.popMsgQueue(true, -1);

	cout << "Tardo " << tiempo << " segundos aproximadamente." << endl;

	cin.ignore();
}