#include"Simulation.h"
#include"Person.h"

// Variables de debugging
//#define debug_locale 1
//#define debug_infect 1

// Constantes de estados
#pragma warning(disable:4996)
#define healthy 0
#define infected 1
#define recovered 2
#define death 3

string Simulation::truncateAndFormat(double val, int cut) {
	string normal = to_string(val);
	if (normal.size() > 3) {
		return normal.substr(0, normal.size() - cut);
	}
	return normal;
}

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

	pushMsgQueueln("");
	pushMsgQueueln("-- Civilizacion inicializada --");
	pushMsgQueueln("");
	pushMsgQueueln("Numero de hilos utilizados:" + truncateAndFormat(simulation_core_num,7));
	pushMsgQueueln("Numero de personas en la simulacion: " + truncateAndFormat(simulation_number_people,7));
	pushMsgQueueln("Dimension del espacio:" + truncateAndFormat(simulation_room_size,7) + "x" + truncateAndFormat(simulation_room_size,7));

	pushMsgQueueln("Porcentaje de infeccion inicial:" + truncateAndFormat(chance_infect_init,7) + "%");
	pushMsgQueueln("Probabilidad de infeccion:" + truncateAndFormat(chance_infect_pertick,7) + "%");
	pushMsgQueueln("Probabilidad de recuperacion:" + truncateAndFormat(chance_recover_pertick,7) + "%");
	pushMsgQueueln("Duracion de la enfermedad en estados:" + to_string(death_counter_max) + " semanas");

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

void Simulation::run(int simulation_tick_limit_param) {

	double percent_healthy = 0;
	double percent_infected = 0;
	double percent_recovered = 0;
	double percent_death = 0;

	double prom_healthy = 0;
	double prom_infected = 0;
	double prom_recovered = 0;
	double prom_death = 0;

	for (int estado = 1; estado <= simulation_tick_limit_param; estado++) {

		// Actualice las estadisticas con la información del estado previo

		prom_healthy += stats_healthy;
		prom_infected += stats_infected;
		prom_recovered += stats_recovered;
		prom_death += stats_death;


		percent_healthy = ((float)stats_healthy / simulation_number_people * 100);
		percent_infected = ((float)stats_infected / simulation_number_people * 100);
		percent_recovered = ((float)stats_recovered / simulation_number_people * 100);
		percent_death = ((float)stats_death / simulation_number_people * 100);

		pushMsgQueueln(
			"Estado(" + to_string(estado) +
			") Sanas:" + to_string(stats_healthy) + "(" + truncateAndFormat(percent_healthy, 4) + "%)" + "(Prom:" + truncateAndFormat(prom_healthy / estado, 7) + ")" +
			", Enfermas:" + to_string(stats_infected) + "(" + truncateAndFormat(percent_infected, 4) + "%)" + "(Prom:" + truncateAndFormat(prom_infected / estado, 7) + ")" +
			", Recuperadas:" + to_string(stats_recovered) + "(" + truncateAndFormat(percent_recovered, 4) + "%)" + "(Prom:" + truncateAndFormat(prom_recovered / estado, 7) + ")" +
			", Muertas:" + to_string(stats_death) + "(" + truncateAndFormat(percent_death, 4) + "%)" + "(Prom:" + truncateAndFormat(prom_death / estado, 7) + ")");

		// Termine si ya se estabilizo

		if (stats_infected == 0) {
			pushMsgQueueln("El sistema se ha estabilizado en el estado(" + to_string(estado)+")");
			break;
		}

		Person* pointer;

		// Paralelice la infección que surge con los datos previos
		#pragma omp parallel for num_threads(simulation_core_num) private(pointer)
		for (int i = 0; i < simulation_number_people; ++i) {
			pointer = &people.at(i);

			if (pointer->getState() != death) {
				if (pointer->getState() == healthy) {

					for (int i = 0; i < roomstate_infected[pointer->getX()][pointer->getY()]; i++) {
						if (rand() % 101 < chance_infect_pertick) {
							pointer->setState(infected);
							roomstate_infected[pointer->getX()][pointer->getY()]++;
							#pragma omp atomic
								stats_infected++;
							#pragma omp atomic
								stats_healthy--;
							break;
						}
					}

				}
				else if (pointer->getState() == infected) {
					if (pointer->infectedForTooLong(death_counter_max - 2)) {
						if (rand() % 101 < chance_recover_pertick) {
							pointer->setState(recovered);
							roomstate_infected[pointer->getX()][pointer->getY()]--;
							roomstate_recovered[pointer->getX()][pointer->getY()]++;
							#pragma omp atomic
								stats_infected--;
							#pragma omp atomic
								stats_recovered++;
						}
						else {
							pointer->setState(death);
							roomstate_infected[pointer->getX()][pointer->getY()]--;
							#pragma omp atomic
								stats_infected--;
							#pragma omp atomic
								stats_death++;
						}
					}

				}

			}

		}

		// Paralelice el movimiento de las personas
		#pragma omp parallel for num_threads(simulation_core_num) private(pointer)
		for (int i = 0; i < simulation_number_people; ++i) {
			pointer = &people.at(i);

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
				pointer->setLocation(pointer->getX() + xdisplace, pointer->getY() + ydisplace, simulation_room_size);
				roomstate_infected[pointer->getX()][pointer->getY()]++;
			}else if (pointer->getState() == recovered) {
					roomstate_recovered[pointer->getX()][pointer->getY()]--;
					pointer->setLocation(pointer->getX() + xdisplace, pointer->getY() + ydisplace, simulation_room_size);
					roomstate_recovered[pointer->getX()][pointer->getY()]++;
			}else {
				pointer->setLocation(pointer->getX() + xdisplace, pointer->getY() + ydisplace, simulation_room_size);
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

void Simulation::popMsgQueue(int howmuch) {

	char fecha_string[25];
	char tiempo_string[25];

	time_t curr_time;
	tm * curr_tm;
	time(&curr_time);
	curr_tm = localtime(&curr_time);

	strftime(fecha_string, 50, "%e.%m.%y_", curr_tm);
	strftime(tiempo_string, 50, "%H.%M.%S", curr_tm);

	string archivo = "Civilizacion_";
	archivo.append(fecha_string);
	archivo.append(tiempo_string);
	archivo = archivo + ".txt";

	std::ofstream out(archivo);
	
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
		out << msg;

		coutQueue.pop_front();
	}

	out.close();



}