#pragma once
#include<iostream>
#include<vector>
#include<queue>
#include<string>
#include"Person.h"
using namespace std;

class Simulation {
public:

	static float infect_chance_init;
	static float infect_chance_pertick;
	static float recover_chance_pertick;
	static float death_chance_pertick;
	static int stats_healthy;
	static int stats_infected;
	static int stats_recovered;
	static int stats_death;

	int stats_prom_healthy_count;
	int stats_prom_infected_count;
	int stats_prom_recovered_count;
	int stats_prom_death_count;

	int number_people;
	int room_size;

	int simulation_tick_limit;

	int core_num;

	vector<Person> people;
	deque<string> coutQueue;

	int** people_num_infected;
	int** people_num_recovered;

	void start(int, int, float, float, float, float, int, int);
	void run(int);

	void pushMsgQueue(string);
	void popMsgQueue(bool, int);
	void cleanMemory();
private:
	void makePeople(); 
	void makeRoom();
};