#include "Initialization.h"
int main()
{
	//initializaion start(number of band , number of SUs , Probability of PU on, TIME SLOT)
	Initialization Start(100, 40, 0.4, 100000);
	Start.Initialize();
	system("pause");
}