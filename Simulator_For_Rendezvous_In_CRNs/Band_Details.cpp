#include "Band_Details.h"
Band_Details::Band_Details(double ProbOn)
{
	//std::random_device rand_dev;
	//std::mt19937 generator(rand_dev());
	//std::default_random_engine generator(1);
	//std::uniform_real_distribution<double> distr(0.00, 1.00);
	ProbON = ProbOn;
	PUOFF = (double(rand()) / double(RAND_MAX)) >= ProbON;
	//PUOFF = distr(generator) >= 0.8;
	//std::cout << distr(generator) << " " << ProbON;
	if (ProbON == 1)											// This solves the problem with extreme values 
		PUOFF = false;
}
Band_Details::~Band_Details()
{
}

void Band_Details::clearPacket()
{
	std::vector<int>().swap(packetVsID);
}

bool Band_Details::isEmpty() const
{
	return PUOFF;
}
bool Band_Details::getProbON() const
{
	return ProbON;
}

void Band_Details::setState(bool State)
{
	PUOFF = State;
}


