#include"Simulation.h"
#include"Person.h"

unsigned int threads = std::thread::hardware_concurrency();

void Simulation::start(int number_people_param, int room_size_param, float infect_chance_pertick_param, float recover_chance_pertick_param, float death_chance_pertick_param, int simulation_tick_limit_param) {
	

	number_people = number_people_param;
	room_size = room_size_param;
	infect_chance_pertick = infect_chance_pertick_param;
	recover_chance_pertick = recover_chance_pertick_param;
	death_chance_pertick = death_chance_pertick_param;
	
	cout << "Inicializacion de una nueva simulacion." << endl;
	cout << "Numero de nucleos disponibles: " << threads << endl;
	cout << "Numero de nacimientos de personas: " << number_people_param << endl;
	cout << "Dimension del espacio: " << room_size << "x" << room_size << endl;
	cout << "Probabilidad de infeccion: " << infect_chance_pertick*100 << "%" << endl;
	cout << "Probabilidad de recuperacion: " << recover_chance_pertick * 100 << "%" << endl;
	cout << "Probabilidad de muerte: " << death_chance_pertick * 100 << "%" << endl;

	makeRoom();
	makePeople();

	cleanMemory();
}


void Simulation::makeRoom() {

	used_cells = new int*[room_size];

	for (int i = 0; i < room_size; ++i) {
		used_cells[i] = new int[room_size];
	}

	for (size_t i = 0; i < room_size; ++i){
		for (size_t j = 0; j < room_size; ++j){
			used_cells[i][j] = 0;
		}
	}

}

void Simulation::cleanMemory() {

	for (int i = 0; i < room_size; ++i) {
		delete used_cells[i];
	}

	delete used_cells;

}

void Simulation::makePeople() {

	for (int i = 0; i < number_people; ++i) {
		Person persona_actual;
		people.push_back(persona_actual);
	}

}

void Simulation::run(int simulation_tick_limit_param) {
	simulation_tick_limit = simulation_tick_limit_param;

	cout << "Se ejecutaran " << simulation_tick_limit << " semanas." << endl;

	simulation_tick_counter = 0;

}