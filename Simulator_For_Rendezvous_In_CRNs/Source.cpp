#include "Initialization.h"
#include "Rendezvous_Algorithm.h"
#include"SecondaryUser.h"
int main()
{
	//initializaion start(number of band , number of SUs , Probability of PU on) 
	Initialization Start(100, 10, 0.2);
	Start.Initialize();
	system("pause");
}

