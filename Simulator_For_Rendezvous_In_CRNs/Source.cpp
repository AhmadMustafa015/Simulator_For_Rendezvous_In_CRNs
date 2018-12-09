#include "Initialization.h"
int main()
{
	//initializaion start(number of band , number of SUs , Probability of PU on) 
	Initialization Start(100, 20 , 0.2,1000);
	Start.Initialize();
	system("pause");
}