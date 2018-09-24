#pragma once
#include<iostream>
#include<vector>
#include<thread>
#include"Person.h"
using namespace std;

class Simulation {
public:
	int stats_infected;
	int stats_healthy;
	int stats_recovered;
	int stats_death;

	int number_people;
	int room_size;

	float infect_chance_pertick;
	float recover_chance_pertick;
	float death_chance_pertick;

	int simulation_tick_counter;
	int simulation_tick_limit;

	vector<Person> people;
	int** used_cells;

	void start(int, int, float, float, float, int);
	void run(int);
private:
	void makePeople();
	void makeRoom();
	void cleanMemory();
};