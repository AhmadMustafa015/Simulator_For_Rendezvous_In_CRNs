#include "Rendezvous_Algorithm.h"
std::random_device rand_dev5;
std::mt19937 generator5(rand_dev5());


Rendezvous_Algorithm::Rendezvous_Algorithm(int initialBand, Transmitter &Tx, std::vector<Band_Details> &Bands, int ID)
{

	distance = 2 * Tx.numberOfRadio;
	upperBound1 = initialBand + distance / 2;
	lowerBound1 = initialBand + 1;
	upperBound2 = initialBand - 1;
	lowerBound2 = initialBand - distance / 2;
	channelHoppingSequence.resize(Tx.numberOfRadio);
	for (int i = 0; i < Tx.numberOfRadio; i++)
	{
		channelHoppingSequence[i] = lowerBound1 + (i)* ceil(distance / Tx.numberOfRadio);
		if (channelHoppingSequence[i] == initialBand)
			++channelHoppingSequence[i];
		if (Tx.scanningBands(Bands, channelHoppingSequence[i]))
			emptyBands.push_back(i);
	}
	timeSlot++;

}



Rendezvous_Algorithm::~Rendezvous_Algorithm()
{
}

void Rendezvous_Algorithm::ourAlgorithmTx(int initialBand, Transmitter &Tx, std::vector<Band_Details> &Bands, int ID)
{
	rendezvous = false;
	while (!rendezvous)
	{
		for (int i = 0; i < Tx.numberOfRadio; i++)
		{
			channelHoppingSequence[i] = lowerBound1 + (i)* ceil(distance / Tx.numberOfRadio);
			if (channelHoppingSequence[i] == initialBand)
				++channelHoppingSequence[i];
			if (Tx.scanningBands(Bands, channelHoppingSequence[i]))
				emptyBands.push_back(i);
		}
		std::uniform_int_distribution<int> distr(0, emptyBands.size()-1);
		if (!emptyBands.empty())
		{
			radioSendPacket = distr(generator5);
			Tx.sendPacket(Bands[channelHoppingSequence[radioSendPacket]], ID, emptyBands[radioSendPacket]);
		}
		timeSlot++;
		int temp = 0;
		for (int i = 0; i < Tx.numberOfRadio; i++)
		{
			if (emptyBands.empty() || std::find(emptyBands.begin(), emptyBands.end(), i) == emptyBands.end())
			{
				temp = channelHoppingSequence[i];
				//++channelHoppingSequence[i];

				if (channelHoppingSequence[i]+1 == upperBound2 || std::find(channelHoppingSequence.begin 
					, channelHoppingSequence.end, (channelHoppingSequence[i]+1)) != channelHoppingSequence.end())
				{
					for (int i = 0; i < (Tx.numberOfRadio - 1); i++)
					{
						if (find(channelHoppingSequence.begin(), channelHoppingSequence.end(),
							(lowerBound1 + 2 * i + 1)) == channelHoppingSequence.end() && (lowerBound1 + 2 * i + 1) != initialBand)
						{
							channelHoppingSequence[i] = lowerBound1 + 2 * i + 1;
							if (Tx.scanningBands(Bands, channelHoppingSequence[i]))
								emptyBands.push_back(i);
						}
					}
				}
				else
				{
					channelHoppingSequence[i]++;
					if (Tx.scanningBands(Bands, channelHoppingSequence[i]))
						emptyBands.push_back(i);
				}
			}
			else
			{
				std::uniform_int_distribution<int> distr(0, emptyBands.size()-1);
				radioSendPacket = distr(generator5);
				Tx.sendPacket(Bands[channelHoppingSequence[radioSendPacket]], ID, emptyBands[radioSendPacket]);
			}
		}
		timeSlot++;

		if (timeSlot > 1)


			/*for (int i = 0; i < numberOfRadios; i++)
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
				}*/

	}
}
}
