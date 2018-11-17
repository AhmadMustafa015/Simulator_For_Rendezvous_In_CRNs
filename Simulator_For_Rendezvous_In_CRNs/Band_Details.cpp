#include "Band_Details.h"

Band_Details::Band_Details(double ProbOn)
{
	std::random_device rand_dev;
	std::mt19937 generator(rand_dev());
	std::uniform_real_distribution<double> distr(0.00, 1.00);
	ProbON = ProbOn;
	//PUOFF = (double(rand()) / double(RAND_MAX)) >= ProbON;
	PUOFF = distr(generator) >= ProbON;
	std::cout << PUOFF << " ";
	if (ProbON == 1)											// This solves the problem with extreme values 
		PUOFF = false;
}
Band_Details::~Band_Details()
{
}

bool Band_Details::isEmpty() const
{
	return PUOFF;
}

void Band_Details::randomPUState()
{
	PUOFF = (double(rand()) / double(RAND_MAX)) >= ProbON;
	if (ProbON == 1)
		PUOFF = false;
}

bool Band_Details::getProbON() const
{
	return ProbON;
}
DeterministicBand::DeterministicBand(double T, int B)
{
	timeVSuccessfulReq = T;
	successfulVsTimePUActiveForBandN = B;
}
bool DeterministicBand::isEmpty(double T, int bandN) const
{
	if (T < timeVSuccessfulReq)
		return true;
	else
	{
		if (bandN > successfulVsTimePUActiveForBandN)
			return true;
		else
		{
			return false;
		}
	}
}

