#pragma once
#include<iostream>
#include<queue>
#include<string>
#include<ctime>
using namespace std;

class Person{
public:
	int getX();
	int getY();
	int getState();
	int getDebugID();
	void setState(int);
	void setLocation(int, int, int);
	bool infectedForTooLong(int);
	Person(int, int);

private:
	int debug_id;
	int state_current;
	int local_x;
	int local_y;
	int infected_ticks;
};