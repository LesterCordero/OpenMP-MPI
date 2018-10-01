#include <iostream>
#include <chrono>
#include "Person.h"
#include "Simulation.h"
#include <omp.h>
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

	// Genera una semilla para los números random
	srand(time(NULL));

	// Construya la simulación
	Simulation sim;

	sim.start(omp_get_num_procs(), 50, 1500, .10f, 0.10f, .01f, .20f);

	// Tome el tiempo y ejecute la simulación
	tiempo_inicio();
	sim.run(40);
	tiempo_final();

	// Limpie la memoria e imprima al archivo
	sim.cleanMemory();
	sim.popMsgQueue(true, -1);
	cout << "Tardo " << tiempo << " segundos aproximadamente." << endl;

	// Fin del programa
	cin.ignore();
}