#include "Initialization.h"
int main()
{
	//initializaion start(number of band , number of SUs , Probability of PU on, TIME SLOTS)
	Initialization Start(100, 40, 0, 100000);
	Start.Initialize();
	system("pause");
}