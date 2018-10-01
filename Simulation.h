#pragma once
#include<iostream>
#include<vector>
#include<queue>
#include<string>
#include"Person.h"
using namespace std;

class Simulation {
public:
	void start(int, int, int, float, float, float, float);
	void run(int);
	void pushMsgQueueln(string);
	void pushMsgQueue(string);
	void popMsgQueue(bool, int);
	void cleanMemory();
private:

	float chance_infect_init;
	float chance_infect_pertick;
	float chance_recover_pertick;
	float chance_death_pertick;

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
	int simulation_tick_limit;
	int simulation_core_num;

	int** roomstate_infected;
	int** roomstate_recovered;

	vector<Person> people;
	deque<string> coutQueue;

	void makePeople(); 
	void makeRoom();
};