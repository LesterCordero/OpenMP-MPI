#pragma once
#include<iostream>
#include<queue>
#include<string>
#include<ctime>
using namespace std;

class Person{
public:
	static const int state_healthy = 0;
	static const int state_infected = 1;
	static const int state_recovered = 2;
	static const int state_dead = 3;

	int debug_id;
	int state_current;
	int local_x;
	int local_y;

	int getX();
	int getY();

	Person(int);
	void updateMe(int** , int**);
	void doMovement(int** , int**);
};