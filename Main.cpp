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


	// Construya la simulación
	Simulation sim;
	sim.start(40, 500, .20f, 0.10f, 0.15f, 0.25f, 100, cores);

	// Tome el tiempo y ejecute la simulación
	tiempo_inicio();
	sim.run(10);
	tiempo_final();

	// Limpie la memoria e imprima al archivo
	sim.cleanMemory();
	sim.popMsgQueue(true, -1);
	cout << "Tardo " << tiempo << " segundos aproximadamente." << endl;

	// Fin del programa
	cin.ignore();
}