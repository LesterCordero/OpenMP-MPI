#include"Simulation.h"
#include"Person.h"
#include<omp.h>

// Variables de debugging
//#define debug_locale 1
//#define debug_infect 1

// Constantes de estados
#define healthy 0
#define infected 1
#define recovered 2
#define death 3

void Simulation::start(int p1, int p2, int p3, int p4, int p5, int p6, int p7) {

	// Carge los parámetros a la simulación 

	simulation_core_num = p1;
	simulation_room_size = p2;
	simulation_number_people = p3;
	chance_infect_init = p4;
	chance_infect_pertick = p5;
	chance_recover_pertick = p6;
	death_counter_max = p7;

	// Prepare los stats por defecto de la simulacion

	stats_healthy = simulation_number_people * (100 - chance_infect_init)/100;
	stats_infected = simulation_number_people - stats_healthy;
	stats_recovered = 0;
	stats_death = 0;

	// Imprima los datos de entrada

	cout << "Inicializacion de una nueva simulacion." << endl;
	cout << "Numero de nucleos disponibles: " << simulation_core_num << endl << endl;

	cout << "Numero de personas inciales: " << simulation_number_people << endl;
	cout << "Dimension del espacio: " << simulation_room_size << "x" << simulation_room_size << endl;
	cout << "Porcentaje de infeccion inicial: " << chance_infect_init << "%" << endl << endl;

	cout << "Probabilidad de infeccion: " << chance_infect_pertick << "%" << endl;
	cout << "Probabilidad de recuperacion: " << chance_recover_pertick << "%" << endl;
	cout << "Semanas enfermo para morir: " << death_counter_max  << " semanas" << endl << endl;

	// Cree la habitación y las personas

	makeRoom();
	makePeople();
}


void Simulation::makeRoom() {

	roomstate_infected = new int*[simulation_room_size];
	roomstate_recovered = new int*[simulation_room_size];

	for (int i = 0; i < simulation_room_size; ++i) {
		roomstate_infected[i] = new int[simulation_room_size];
		roomstate_recovered[i] = new int[simulation_room_size];
	}

	for (size_t i = 0; i < simulation_room_size; ++i) {
		for (size_t j = 0; j < simulation_room_size; ++j) {
			roomstate_infected[i][j] = 0;
			roomstate_recovered[i][j] = 0;
		}
	}

}

void Simulation::cleanMemory() {

	for (int i = 0; i < simulation_room_size; ++i) {
		delete roomstate_infected[i];
		delete roomstate_recovered[i];
	}

	delete roomstate_infected;
	delete roomstate_recovered;
}

void Simulation::makePeople() {

	int j = stats_infected;
	for (int i = 0; i < simulation_number_people; ++i) {
		Person persona_actual(i, simulation_room_size);

		if (j > 0) {
			persona_actual.setState(infected);
			j--;
			roomstate_infected[persona_actual.getX()][persona_actual.getY()]++;
		}

		people.push_back(persona_actual);
	}

}

string Simulation::truncateAndFormat(double val, int cut) {
	string normal = to_string(val);
	if (normal.size() > 3) {
		return normal.substr(0, normal.size() - cut);
	}
	return normal;

	
}

int Simulation::getRandom() {
	return (25234 + 17 * omp_get_thread_num());
}

void Simulation::run(int simulation_tick_limit_param) {
	simulation_tick_limit = simulation_tick_limit_param;
	int estado_num = 0;

	pushMsgQueueln("Ejecutando " + to_string(simulation_tick_limit_param) + " veces.");

	double percent_healthy = 0;
	double percent_infected = 0;
	double percent_recovered = 0;
	double percent_death = 0;

	prom_healthy = 0;

	while (simulation_tick_limit > 0) {
		simulation_tick_limit--;
		estado_num++;

		prom_healthy += stats_healthy;
		prom_infected += stats_infected;
		prom_recovered += stats_recovered;
		prom_death += stats_death;

		percent_healthy = ((float)stats_healthy / simulation_number_people * 100);
		percent_infected = ((float)stats_infected / simulation_number_people * 100);
		percent_recovered = ((float)stats_recovered / simulation_number_people * 100);
		percent_death = ((float)stats_death / simulation_number_people * 100);

		pushMsgQueueln(
			"Estado(" + to_string(estado_num) +
			") Sanas:" + to_string(stats_healthy) + "(" + truncateAndFormat(percent_healthy, 4) + "%)" + "(Prom:" + to_string(prom_healthy/estado_num) + ")" +
			", Enfermas:" + to_string(stats_infected) + "(" + truncateAndFormat(percent_infected, 4) + "%)" + "(Prom:" + to_string(prom_infected / estado_num) + ")" +
			", Recuperadas:" + to_string(stats_recovered) + "(" + truncateAndFormat(percent_recovered, 4) + "%)" + "(Prom:" + to_string(prom_recovered / estado_num) + ")" +
			", Muertas: " + to_string(stats_death) + "(" + truncateAndFormat(percent_death, 4) + "%)" + "(Prom:" + to_string(prom_death / estado_num) + ")");


#ifdef debug_locale
		pushMsgQueueln(" ");
		pushMsgQueueln("       DEBUG: Localizacion de las personas:");
		for (int i = 0; i < simulation_number_people; ++i) {
			pushMsgQueueln("       Persona #" + to_string(people.at(i).getDebugID() + 1) + " (x:" + to_string(people.at(i).getX()) + ", y:" + to_string(people.at(i).getY()) + ")");
		}
#endif

#ifdef debug_infect
		pushMsgQueueln(" ");
		pushMsgQueueln("       DEBUG: Numero de infectados por celda:");
		for (int xx = 0; xx < simulation_room_size; xx++) {
			pushMsgQueue("       ");
			for (int yy = 0; yy < simulation_room_size; yy++) {
				pushMsgQueue(" " + to_string(roomstate_infected[xx][yy]));
			}
			pushMsgQueueln("");
		}
#endif


		int intentos = omp_get_num_threads();
		Person* pointer;

#pragma omp parallel for num_threads(intentos) private(pointer)
		for (int i = 0; i < simulation_number_people; ++i) {

			pointer = &people.at(i);

			if (pointer->getState() != death) {

				if (pointer->getState() == healthy) {
					
					for (int i = 0; i < roomstate_infected[pointer->getX()][pointer->getY()]; i++) {

						if (rand() % 101 < chance_infect_pertick) {
							pointer->setState(infected);
							roomstate_infected[pointer->getX()][pointer->getY()]++;
							stats_infected++;
							stats_healthy--;
							break;
						}

					}

				}else if (pointer->getState() == infected) {
					if (pointer->infectedForTooLong(death_counter_max-2)) {
						if (rand() % 101 < chance_recover_pertick) {
							pointer->setState(recovered);
							roomstate_infected[pointer->getX()][pointer->getY()]--;
							roomstate_recovered[pointer->getX()][pointer->getY()]++;
							stats_infected--;
							stats_recovered++;
						}else {
							pointer->setState(death);
							roomstate_infected[pointer->getX()][pointer->getY()]--;
							stats_infected--;
							stats_death++;
						}
					}

				}

				int xdisplace = 1;
				int ydisplace = 1;

				if (rand() % 100 > 50) {
					xdisplace = -1;
				}

				if (rand() % 100 > 50) {
					ydisplace = -1;
				}

				if (pointer->getState() == infected) {
					roomstate_infected[pointer->getX()][pointer->getY()]--;
				}

				pointer->setLocation(pointer->getX() + xdisplace, pointer->getY() + ydisplace, simulation_room_size);

				if (pointer->getState() == infected) {
					roomstate_infected[pointer->getX()][pointer->getY()]++;
				}



			}

		}

	}
}

void Simulation::pushMsgQueue(string msg) {
	coutQueue.push_back(msg);
}

void Simulation::pushMsgQueueln(string msg) {
	coutQueue.push_back(msg + "\n");
}

void Simulation::popMsgQueue(bool writeToFile, int howmuch) {
	if (howmuch == -1) {
		howmuch = coutQueue.size();
	}
	for (int i = 0; i < howmuch; i++) {
		string msg = coutQueue.front();
#ifdef debug_infect
		if (msg == " 0") {
			msg = " -";
		}
#endif
		cout << msg;
		coutQueue.pop_front();
	}

}