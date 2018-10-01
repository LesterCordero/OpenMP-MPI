#include"Simulation.h"
#include"Person.h"

// Variables de debugging
//#define debug_locale 1
#define debug_infect 1
//#define debug_notify_infect

// Constantes de estados
#define healthy 0
#define infected 1
#define recovered 2
#define death 3

void Simulation::start(int p1, int p2, int p3, float p4, float p5, float p6, float p7) {
	
	// Carge los parámetros a la simulación 

	simulation_core_num = p1;
	simulation_room_size = p2;
	simulation_number_people = p3;
	chance_infect_init = p4;
	chance_infect_pertick = p5;
	chance_recover_pertick = p6;
	chance_death_pertick = p7;
	
	// Prepare los stats por defecto de la simulacion

	stats_healthy = simulation_number_people * (1 - chance_infect_init);
	stats_infected = simulation_number_people - stats_healthy;
	stats_recovered = 0;
	stats_death = 0;

	//prom_healthy_count = Simulation::stats_healthy;
	//prom_infected_count = Simulation::stats_infected;
	//prom_recovered_count = 0;
	//prom_death_count = 0;

	// Imprima los datos de entrada

	cout << "Inicializacion de una nueva simulacion." << endl;
	cout << "Numero de nucleos disponibles: " << simulation_core_num << endl << endl;

	cout << "Numero de personas inciales: " << simulation_number_people << endl;
	cout << "Dimension del espacio: " << simulation_room_size << "x" << simulation_room_size << endl;
	cout << "Porcentaje de infeccion inicial: " << chance_infect_init * 100 << "%" << endl << endl;

	cout << "Probabilidad de infeccion: " << chance_infect_pertick *100 << "%" << endl;
	cout << "Probabilidad de recuperacion: " << chance_recover_pertick * 100 << "%" << endl;
	cout << "Probabilidad de muerte: " << chance_death_pertick * 100 << "%" << endl << endl;

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

	for (size_t i = 0; i < simulation_room_size; ++i){
		for (size_t j = 0; j < simulation_room_size; ++j){
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
	simulation_tick_limit = simulation_tick_limit_param;
	int estado_num = 0;

	pushMsgQueueln("Ejecutando " + to_string(simulation_tick_limit_param) + " veces.");

	while (simulation_tick_limit > 0) {
		simulation_tick_limit--;
		estado_num++;

		pushMsgQueueln(
		"Estado(" + to_string(estado_num) +
		") Sanas:" + to_string(stats_healthy) +
		", Enfermas:" + to_string(stats_infected) +
		", Recuperadas:" + to_string(stats_recovered) +
		", Muertas: " + to_string(stats_death));

		#ifdef debug_locale
			pushMsgQueueln(" ");
			pushMsgQueueln("       DEBUG: Localizacion de las personas:");
		#endif

		for (int i = 0; i < simulation_number_people; ++i) {

			Person* pointer;
			pointer = &people.at(i);

			if (pointer->getState() != death) {
			
				if (pointer->getState() == healthy) {
					for (int i = 0; i < roomstate_infected[pointer->getX()][pointer->getY()]; i++) {
						if (rand() % 101 < chance_infect_pertick * 100) {
							#ifdef debug_notify_infect
								pushMsgQueueln("       DEBUG: Se infectara una persona en el siguiente estado.");
							#endif

							pointer->setState(infected);
							roomstate_infected[pointer->getX()][pointer->getY()]++;
							stats_infected++;
							stats_healthy--;
							break;
						}
					}

				}

				//update_percent()

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

				#ifdef debug_locale
					pushMsgQueueln("       Persona #"+to_string(pointer->getDebugID()+1)+" (x:" + to_string(pointer->getX()) + ", y:" + to_string(pointer->getY()) + ")");
				#endif

			}

		}

		#ifdef debug_infect
		pushMsgQueueln(" ");
		pushMsgQueueln("       DEBUG: Numero de infectados por celda:");
		for (int xx = 0; xx < simulation_room_size; xx++) {
			pushMsgQueue("       ");
			for (int yy = 0; yy < simulation_room_size; yy++) {
				pushMsgQueue(to_string(roomstate_infected[xx][yy]));
			}
			pushMsgQueueln("");
		}
		#endif

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
		if (msg == "0") {
			msg = "-";
		}
#endif
		cout << msg;
		coutQueue.pop_front();
	}
	
}
