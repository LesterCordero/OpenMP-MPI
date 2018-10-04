#include<iostream>
#include<chrono>
#include"Person.h"
#include"Simulation.h"
#include<omp.h>
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

	//simulation_core_num, simulation_room_size, simulation_number_people; chance_infect_init; chance_infect_pertick; chance_recover_pertick; death_counter_max;
	//sim.start(omp_get_num_procs(), 100, 500000, 0.10f, 0.075f, .01f, 50);

	float personas = 0, tam = 0, propInfectInicial = 0, propInfect = 0, propRecover = 0, duracionEnfermedad = 0;
	
	cout << "Numero de personas en la civilizacion:" << endl;
	cin >> personas;

	cout << "Largo/Ancho del espacio vectorial:" << endl;
	cin >> tam;

	cout << "Porcentaje de infeccion inicial (numero ENTERO de 0 a 10):" << endl;
	cin >> propInfectInicial;

	cout << "Probabilidad de infeccion por estado (numero ENTERO entre 0 a 100):" << endl;
	cin >> propInfect;

	cout << "Probabilidad de recuperacion al terminar la enfermedad (numero ENTERO entre 0 a 100):" << endl;
	cin >> propRecover;

	cout << "Duracion de la enfermedad (numero ENTERO de estados enfermos):" << endl;
	cin >> duracionEnfermedad;

	sim.start(omp_get_num_procs(), tam, personas, propInfectInicial, propInfect, propRecover, duracionEnfermedad);

	cout << sim.getRandom() << endl;

	// Tome el tiempo y ejecute la simulación
	tiempo_inicio();
	sim.run(200);
	tiempo_final();

	// Limpie la memoria e imprima al archivo
	sim.cleanMemory();
	sim.popMsgQueue(true, -1);
	cout << "Tardo " << tiempo << " segundos aproximadamente." << endl;

	// Fin del programa
	cin >> personas;
}