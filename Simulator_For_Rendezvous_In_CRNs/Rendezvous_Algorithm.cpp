#include "Rendezvous_Algorithm.h"
std::random_device rand_dev5;
std::mt19937 generator5(rand_dev5());


Rendezvous_Algorithm::Rendezvous_Algorithm()
{
}


Rendezvous_Algorithm::~Rendezvous_Algorithm()
{
}

void Rendezvous_Algorithm::ourAlgorithm(int initialBand, std::vector<int> &availableBand, int numberOfRadios)
{
	distance = 2 * numberOfRadios;
	upperBound1 = initialBand;
	lowerBound1 = initialBand - distance / 2;
	upperBound2 = initialBand;
	lowerBound2 = initialBand + distance / 2;
	channelHoppingSequence.resize(numberOfRadios);
		for (int i = 0; i < numberOfRadios; i++)
		{
			channelHoppingSequence[i] = lowerBound1 + (i) * ceil(distance / numberOfRadios);
			if (channelHoppingSequence[i] == initialBand)
				++channelHoppingSequence[i];
		}
		for (int i = 0; i < numberOfRadios; i++)
		{
			while (std::find(availableBand.begin(), availableBand.end(), channelHoppingSequence[i]) == availableBand.end()
				&& channelHoppingSequence[i] < channelHoppingSequence[i + 1])
				++channelHoppingSequence[i];
			while (std::find(availableBand.begin(), availableBand.end(), channelHoppingSequence[i]) == availableBand.end())
			{
				upperBound1 = lowerBound1;
				lowerBound1 = lowerBound1 - distance / 2;
				upperBound2 = lowerBound2;
				lowerBound2 = lowerBound2 - distance / 2;
				std::uniform_int_distribution<int> distr(lowerBound1, upperBound1);
				channelHoppingSequence[i] = distr(generator5);
				if (std::find(availableBand.begin(), availableBand.end(), channelHoppingSequence[i]) == availableBand.end())
				{
					std::uniform_int_distribution<int> distr(lowerBound2, upperBound2);
					channelHoppingSequence[i] = distr(generator5);
				}
			}
		}
		timeSlot++;
}
