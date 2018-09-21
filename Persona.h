#pragma once
#include<iostream>
using namespace std;

class Persona{
public:
	const int state_healthy = 0;
	const int state_infected = 1;
	const int state_recovered = 2;
	int state_current;

	Persona(int debug_id);
};