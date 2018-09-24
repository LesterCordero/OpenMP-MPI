#include"Simulation.h"
#include"Person.h"

void Simulation::start(int number_people_param, int room_size_param, float infect_chance_pertick_param, float recover_chance_pertick_param, float death_chance_pertick_param, int simulation_tick_limit_param, int cores) {
	

	number_people = number_people_param;
	room_size = room_size_param;
	infect_chance_pertick = infect_chance_pertick_param;
	recover_chance_pertick = recover_chance_pertick_param;
	death_chance_pertick = death_chance_pertick_param;
	core_num = cores;

	cout << "Inicializacion de una nueva simulacion." << endl;
	cout << "Numero de nucleos disponibles: " << cores << endl;
	cout << "Numero de personas inciales: " << number_people_param << endl;
	cout << "Dimension del espacio: " << room_size << "x" << room_size << endl;
	cout << "Probabilidad de infeccion: " << infect_chance_pertick*100 << "%" << endl;
	cout << "Probabilidad de recuperacion: " << recover_chance_pertick * 100 << "%" << endl;
	cout << "Probabilidad de muerte: " << death_chance_pertick * 100 << "%" << endl;

	makeRoom();
	makePeople();

	cleanMemory();
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

	pushMsgQueue("Debug: Estado(0) [inicial] ");
	for (int i = 0; i < number_people; ++i) {
		Person persona_actual(i);
		pushMsgQueue("       (x:" + to_string(persona_actual.getX()) + ", y:" + to_string(persona_actual.getY())+")");
		people.push_back(persona_actual);
	}

}

void Simulation::run(int simulation_tick_limit_param) {
	simulation_tick_limit = simulation_tick_limit_param;
	int estado_num = 0;
	pushMsgQueue("Debug: Se ejecutaran " + to_string(simulation_tick_limit) + " semanas.");
	
	vector<Person>::iterator it;

	while (simulation_tick_limit > 0) {
		simulation_tick_limit--;
		estado_num++;

		pushMsgQueue("Debug: Estado("+to_string(estado_num)+")");

		//paselo a vector at
		for (it = people.begin(); it != people.end(); it++) {
			it->updateMe();     // Usando los datos del estado previo (t-1)
			it->doMovement();   // Hace el movimiento y lo guardo en el estado actual (t)
			it->doMatrix();     // Actualiza las matrices para el estado actual (t)
			pushMsgQueue("       (x:" + to_string(it->getX()) + ", y:" + to_string(it->getY()) + ")");
		}
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