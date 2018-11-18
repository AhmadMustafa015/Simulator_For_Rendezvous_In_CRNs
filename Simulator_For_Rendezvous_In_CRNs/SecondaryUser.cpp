#include "SecondaryUser.h"

std::random_device rand_dev;
std::mt19937 generator(rand_dev());
SecondaryUser::SecondaryUser()
{
	NumberOfBand = 100;
}
void SecondaryUser::scanningBands(const std::vector<Band_Details> &Bands)
{
	std::uniform_real_distribution<double> distr(0.00, 1.00);
	for (unsigned int i = 0; i < Bands.size(); i++)
	{
		if (Bands[i].isEmpty()) //H0
		{
			emptyBands.push_back(i);
		}
	}
}
void SecondaryUser::bandAllocation(int randomBand)
{
	if (emptyBands.size() != 0) 
	{
		allocatedBand = randomBand;
			//Bands[emptyBands[randomBand]].setOccupants(SUID);
	}

}
void SecondaryUser::emptyAllResult() {
	emptyBands.clear();
}

