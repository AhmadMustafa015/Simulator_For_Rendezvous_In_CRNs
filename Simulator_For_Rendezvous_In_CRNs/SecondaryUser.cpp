#include "SecondaryUser.h"


SecondaryUser::SecondaryUser()
{
	NumberOfBand = 100;
	numOfBandsReqForSUs = 10;
}

SecondaryUser::SecondaryUser(double PF_A, double PM_D, int NumberOfBandint)
	{
	NumberOfBand = NumberOfBandint;
}

void SecondaryUser::scanningBands(const std::vector<Band_Details> &Bands, double asymettric)
{
	std::random_device rand_dev;
	std::mt19937 generator(rand_dev());
	std::uniform_real_distribution<double> distr(0.00, 1.00);
	for (unsigned int i = 0; i < Bands.size(); i++)
	{
		if (Bands[i].isEmpty() && distr(generator) >= asymettric) //H0
		{
			emptyBands.push_back(i);
		}
	}
}
void SecondaryUser::emptyAllResult() {
	emptyBands.clear();
}
void SecondaryUser::selectUserId(const int &getUserId)
{
	UserID = getUserId;
}
