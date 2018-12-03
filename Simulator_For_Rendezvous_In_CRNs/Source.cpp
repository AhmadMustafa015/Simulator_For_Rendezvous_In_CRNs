#include "Initialization.h"
int main()
{
	//initializaion start(number of band , number of SUs , Probability of PU on) 
	Initialization Start(100, 40 , .4);
	Start.Initialize();
	system("pause");
}