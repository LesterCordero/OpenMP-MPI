#include"Simulador.h"
#include"Persona.h"


Simulador::Simulador(int number_people_param, int room_size_param, float infect_chance_pertick_param, float recover_chance_pertick_param, float death_chance_pertick_param, int simulation_tick_limit_param) {
	cout << "Inicializacion de una nueva simulacion." << endl;

	number_people = number_people_param;
	room_size = room_size_param;
	infect_chance_pertick = infect_chance_pertick_param;
	recover_chance_pertick = recover_chance_pertick_param;
	death_chance_pertick = death_chance_pertick_param;
	simulation_tick_limit = simulation_tick_limit_param;

	cout << "Numero de personas: " << number_people_param << endl;
	cout << "Dimension del espacio: " << room_size << "x" << room_size << endl;
	cout << "Probabilidad de infeccion: " << infect_chance_pertick*100 << "%" << endl;
	cout << "Probabilidad de recuperacion: " << recover_chance_pertick * 100 << "%" << endl;
	cout << "Probabilidad de muerte: " << death_chance_pertick * 100 << "%" << endl;
	cout << "Numero de semanas: " << simulation_tick_limit << endl;

	crearPersonas();
}

void Simulador::crearPersonas() {

	for (int i = 0; i < number_people; ++i) {
		Persona persona_actual(i);
		personas.push_back(persona_actual);
	}

}