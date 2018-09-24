#pragma once
#include<iostream>
#include<vector>
#include<queue>
#include<string>
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

	int simulation_tick_limit;

	int core_num;

	vector<Person> people;
	deque<string> coutQueue;

	int** people_num_infected;
	int** people_num_recovered;

	void start(int, int, float, float, float, int, int);
	void run(int);

	void pushMsgQueue(string);
	void popMsgQueue(bool, int);
private:
	void makePeople(); 
	void makeRoom();
	void cleanMemory();
};