#include"Simulation.h"
#include"Person.h"

// Stats globales de la simulación

float Simulation::infect_chance_init = 0;
float Simulation::infect_chance_pertick = 0;
float Simulation::recover_chance_pertick = 0;
float Simulation::death_chance_pertick = 0;
int Simulation::stats_healthy  = 0;
int Simulation::stats_infected;
int Simulation::stats_recovered = 0;
int Simulation::stats_death = 0;

void Simulation::start(int number_people_param, int room_size_param, float infect_chance_pertick_param, float infect_chance_init_param, float recover_chance_pertick_param, float death_chance_pertick_param, int simulation_tick_limit_param, int cores) {
	
	// Carge los parámetros a la simulación 

	number_people = number_people_param;
	room_size = room_size_param;
	core_num = cores;

	Simulation::infect_chance_pertick = infect_chance_pertick_param;
	Simulation::infect_chance_init = infect_chance_init_param;
	Simulation::recover_chance_pertick = recover_chance_pertick_param;
	Simulation::death_chance_pertick = death_chance_pertick_param;
	

	// Prepare los stats default de la simulacion
	// Las variables de tipo stats son estáticas para permitir que puedan ser modificadas por person.cpp sin necesidad de pasar puntero 

	Simulation::stats_healthy = number_people * (1 - infect_chance_init);
	Simulation::stats_infected = number_people - stats_healthy;
	Simulation::stats_recovered = 0;
	Simulation::stats_death = 0;

	stats_prom_healthy_count = Simulation::stats_healthy;
	stats_prom_infected_count = Simulation::stats_infected;
	stats_prom_recovered_count = 0;
	stats_prom_death_count = 0;

	// Imprima los datos de entrada

	cout << "Inicializacion de una nueva simulacion." << endl;
	cout << "Numero de nucleos disponibles: " << cores << endl << endl;

	cout << "Numero de personas inciales: " << number_people_param << endl;
	cout << "Dimension del espacio: " << room_size << "x" << room_size << endl;
	cout << "Porcentaje de infeccion inicial: " << infect_chance_init * 100 << "%" << endl << endl;

	cout << "Probabilidad de infeccion: " << infect_chance_pertick*100 << "%" << endl;
	cout << "Probabilidad de recuperacion: " << recover_chance_pertick * 100 << "%" << endl;
	cout << "Probabilidad de muerte: " << death_chance_pertick * 100 << "%" << endl << endl;

	// Cree la habitación y las personas

	makeRoom();
	makePeople();
}


void Simulation::makeRoom() {

	people_num_infected = new int*[room_size];
	people_num_recovered = new int*[room_size];

	for (int i = 0; i < room_size; ++i) {
		people_num_infected[i] = new int[room_size];
		people_num_recovered[i] = new int[room_size];
	}

	for (size_t i = 0; i < room_size; ++i){
		for (size_t j = 0; j < room_size; ++j){
			people_num_infected[i][j] = 0;
			people_num_recovered[i][j] = 0;
		}
	}

}

void Simulation::cleanMemory() {

	for (int i = 0; i < room_size; ++i) {
		delete people_num_infected[i];
		delete people_num_recovered[i];
	}

	delete people_num_infected;
	delete people_num_recovered;
}

void Simulation::makePeople() {

	pushMsgQueue(
	"Estado(0) Sanas:" + to_string(Simulation::stats_healthy) +
	", Enfermas:" + to_string(Simulation::stats_infected) +
	", Recuperadas:" + to_string(Simulation::stats_recovered) +
	", Muertas: " + to_string(Simulation::stats_death));

	int j = Simulation::stats_infected;
	for (int i = 0; i < number_people; ++i) {
		Person persona_actual(i);

		if (persona_actual.state_current == Person::state_infected) {
			people_num_infected[persona_actual.getX()][persona_actual.getY()] += 1;
		}
		people.push_back(persona_actual);
	}

}

void Simulation::run(int simulation_tick_limit_param) {
	simulation_tick_limit = simulation_tick_limit_param;
	int estado_num = 0;

	while (simulation_tick_limit > 0) {
		simulation_tick_limit--;
		estado_num++;

		for (int i = 0; i < number_people; ++i) {
			people.at(i).updateMe(people_num_infected, people_num_recovered);    // Usando los datos del estado previo (t-1)
			//update_percent()
			people.at(i).doMovement(people_num_infected, people_num_recovered);	 // Actualiza para el estado siguiente	  
			//pushMsgQueue("       (x:" + to_string(people.at(i).getX()) + ", y:" + to_string(people.at(i).getY()) + ")");
		}

		pushMsgQueue(
			"Estado(" + to_string(estado_num) +
			") Sanas:" + to_string(Simulation::stats_healthy) +
			", Enfermas:" + to_string(Simulation::stats_infected) +
			", Recuperadas:" + to_string(Simulation::stats_recovered) +
			", Muertas: " + to_string(Simulation::stats_death));

		}
}

void Simulation::pushMsgQueue(string msg) {
	coutQueue.push_back(msg);
}

void Simulation::popMsgQueue(bool writeToFile, int howmuch) {
	if (howmuch == -1) {
		howmuch = coutQueue.size();
	}
	for (int i = 0; i < howmuch; i++) {
		string msg = coutQueue.front();

		cout << msg << endl;
		coutQueue.pop_front();
	}
	
}