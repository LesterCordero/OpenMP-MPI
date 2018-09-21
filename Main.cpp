#include <vector>
#include <iostream>
#include <string>
#include <chrono> 
#include <omp.h> 
#include "Persona.h"
#include "Simulador.h"
using namespace std;
using namespace std::chrono;

int main() {

	cout << "Simulacion ejecutandose desde github" << endl;

	Simulador sim(50, 500, 0.5f, 0.25f, 0.33f, 100);
	
	cin.ignore();
}