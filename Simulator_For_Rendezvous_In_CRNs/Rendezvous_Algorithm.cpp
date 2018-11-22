#include "Rendezvous_Algorithm.h"
std::random_device rand_dev5;
std::mt19937 generator5(rand_dev5());


Rendezvous_Algorithm::Rendezvous_Algorithm()
{
}


Rendezvous_Algorithm::~Rendezvous_Algorithm()
{
}

bool Rendezvous_Algorithm::ourAlgorithm(int initialBand, std::vector<int> &availableBand, int numberOfRadios)
{
	rendezvous = false;
	distance = 2 * numberOfRadios;
	upperBound1 = initialBand;
	lowerBound1 = initialBand - distance / 2;
	upperBound2 = initialBand;
	lowerBound2 = initialBand + distance / 2;
	while (!rendezvous)
	{
		for (int i = 1; i <= numberOfRadios; i++)
		{
			rendezvousSequence[i] = lowerBound1 + (i - 1) * ceil(distance / numberOfRadios);
			if (rendezvousSequence[i] == initialBand)
				++rendezvousSequence[i];
		}
		for (int i = 1; i <= numberOfRadios; i++)
		{
			while (std::find(availableBand.begin(), availableBand.end(), rendezvousSequence[i]) == availableBand.end()
				&& rendezvousSequence[i] < rendezvousSequence[i + 1])
				++rendezvousSequence[i];
			while (std::find(availableBand.begin(), availableBand.end(), rendezvousSequence[i]) == availableBand.end())
			{
				upperBound1 = lowerBound1;
				lowerBound1 = lowerBound1 - distance / 2;
				upperBound2 = lowerBound2;
				lowerBound2 = lowerBound2 - distance / 2;
				std::uniform_int_distribution<int> distr(lowerBound1, upperBound1);
				rendezvousSequence[i] = distr(generator5);
				if (std::find(availableBand.begin(), availableBand.end(), rendezvousSequence[i]) == availableBand.end())
				{
					std::uniform_int_distribution<int> distr(lowerBound2, upperBound2);
					rendezvousSequence[i] = distr(generator5);
				}
			}
		}
		timeSlot++;
	}
}
