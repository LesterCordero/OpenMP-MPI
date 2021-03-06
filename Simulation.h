#pragma once
#include<iostream>
#include<iomanip>
#include<vector>
#include<fstream>
#include<queue>
#include<string>
#include<omp.h>
#include<ctime>
#include"Person.h"
using namespace std;

class Simulation {
public:
	void start(int, int, int, int, int, int, int);
	void run(int);
	void pushMsgQueueln(string);
	void pushMsgQueue(string);
	void popMsgQueue(int);
	void cleanMemory();

private:

	float chance_infect_init;
	float chance_infect_pertick;
	float chance_recover_pertick;
	int death_counter_max;

	int stats_healthy;
	int stats_infected;
	int stats_recovered;
	int stats_death;

	int prom_healthy;
	int prom_infected;
	int prom_recovered;
	int prom_death;

	int simulation_number_people;
	int simulation_room_size;
	int simulation_core_num;

	int** roomstate_infected;
	int** roomstate_recovered;

	vector<Person> people;
	deque<string> coutQueue;

	void makePeople(); 
	void makeRoom();
	string truncateAndFormat(double, int);
};