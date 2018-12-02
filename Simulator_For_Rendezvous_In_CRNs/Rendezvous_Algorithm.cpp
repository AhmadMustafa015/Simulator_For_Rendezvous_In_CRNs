#include "Rendezvous_Algorithm.h"
//std::random_device rand_dev5;
//std::mt19937 generator5(rand_dev5());

std::default_random_engine generator5(1);
Rendezvous_Algorithm::Rendezvous_Algorithm(int initialBand, Transmitter &Tx, std::vector<Band_Details> &Bands, int ID)
{
	distance = 2 * Tx.numberOfRadio;
	std::cout << distance << " ";
	upperBound1 = initialBand + distance / 2;
	std::cout << upperBound1 << " ";
	lowerBound1 = initialBand + 1;
	upperBound2 = initialBand - 1;
	lowerBound2 = initialBand - distance / 2;
	channelHoppingSequence.resize(Tx.numberOfRadio);
	channelSequence.resize(distance);
	for (int b = lowerBound2; b <= upperBound2; b++)
	{
		channelSequence.push_back(b);
		std::cout << channelSequence[b-lowerBound2] << " ";
		channelSequence[Tx.numberOfRadio + (b - lowerBound2)] = lowerBound1 + (b - lowerBound2);
		std::cout << channelSequence[Tx.numberOfRadio + (b - lowerBound2)] << " ";
	}
	for (int i = 0; i < Tx.numberOfRadio; i++)
	{
		channelHoppingSequence[i] = channelSequence[i * 2];
		std::cout << channelHoppingSequence[i] << " ";
		if (Tx.scanningBands(Bands, channelHoppingSequence[i]))
			radiosWithEmptyBand.push_back(i);
	}
}
Rendezvous_Algorithm::Rendezvous_Algorithm(int initialBand, Receiver & RX, std::vector<Band_Details>& Bands, int ID)
	:twoTimeSlotPassed(RX.numberOfRadio , false)
{
	distance = 2 * RX.numberOfRadio;
	upperBound1 = initialBand + distance / 2;
	lowerBound1 = initialBand + 1;
	upperBound2 = initialBand - 1;
	lowerBound2 = initialBand - distance / 2;
	channelHoppingSequence.resize(RX.numberOfRadio);
	channelSequence.resize(distance);
	for (int b = lowerBound2; b <= upperBound2; b++)
	{
		channelSequence.push_back(b);
		channelSequence[RX.numberOfRadio + (b - lowerBound2)] = lowerBound1 + (b - lowerBound2);
	}
	for (int i = 0; i < RX.numberOfRadio; i++)
	{
		channelHoppingSequence[i] = channelSequence[i * 2];
		if (RX.scanningBands(Bands, channelHoppingSequence[i]))
			radiosWithEmptyBand.push_back(i);
		firstRendezvous =  RX.listening(Bands[channelHoppingSequence[i]], ID);
		twoTimeSlotPassed[i] = true;
	}
}
void Rendezvous_Algorithm::ourAlgorithmTx(int initialBand, Transmitter &Tx, std::vector<Band_Details> &Bands, int ID,int timeSlot)
{
	//if(timeSlot == 4 * ) 
	if (timeSlot % 4 == 0 || (radiosWithEmptyBand.empty() && timeSlot % 2 == 0))
	{
		channelSequence.empty();
		lowerBound1 = upperBound1 + 1;
		upperBound1 += Tx.numberOfRadio;
		upperBound2 = lowerBound2 - 1;
		lowerBound2 -= Tx.numberOfRadio;
		for (int b = lowerBound2; b <= upperBound2; b++)
		{
			channelSequence.push_back(b);
			channelSequence[Tx.numberOfRadio + (b - lowerBound2)] = lowerBound1 + (b - lowerBound2);
		}
		for (int i = 0; i < Tx.numberOfRadio; i++)
		{
			channelHoppingSequence[i] = channelSequence[i * 2];
			if (Tx.scanningBands(Bands, channelHoppingSequence[i]))    
				radiosWithEmptyBand.push_back(i);
		}
	}
	else if(timeSlot > 1)
	{
		for (int i = 0; i < Tx.numberOfRadio; i++)
		{
			if (radiosWithEmptyBand.empty() || std::find(radiosWithEmptyBand.begin(), radiosWithEmptyBand.end(), i) == radiosWithEmptyBand.end())
			{
				//++channelHoppingSequence[i];
				if (channelHoppingSequence[i] == upperBound1 || std::find(channelHoppingSequence.begin()
					, channelHoppingSequence.end(), (channelHoppingSequence[i] + 1)) != channelHoppingSequence.end())
				{
					for (int j = 0; j < (Tx.numberOfRadio); j++)
					{
						if (find(channelHoppingSequence.begin(), channelHoppingSequence.end(),
							channelSequence[j * 2 + 1]) == channelHoppingSequence.end())
						{
							channelHoppingSequence[j] = channelSequence[j * 2 + 1];
							if (Tx.scanningBands(Bands, channelHoppingSequence[j]))
								radiosWithEmptyBand.push_back(j);
						}
					}
				}
				else
				{
					std::vector<int>::iterator itr = std::find(channelSequence.begin(), channelSequence.end(), channelHoppingSequence[i]);
					channelHoppingSequence[i] = channelSequence[std::distance(channelSequence.begin(), itr) + 1];
					if (Tx.scanningBands(Bands, channelHoppingSequence[i]))
						radiosWithEmptyBand.push_back(i);
				}
			}
		}
	}
	if (!radiosWithEmptyBand.empty())
	{
		std::uniform_int_distribution<int> distr(1, radiosWithEmptyBand.size());
		if (radiosWithEmptyBand.size() != 0)
		{
			radioSendPacket = distr(generator5) - 1;
			Tx.sendPacket(Bands[channelHoppingSequence[radioSendPacket]], ID, radiosWithEmptyBand[radioSendPacket]);
		}
	}
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
	radiosWithEmptyBand.clear();
}

bool Rendezvous_Algorithm::ourAlgorithmRx(int initialBand, Receiver & RX, std::vector<Band_Details>& Bands, int ID, int timeSlot)
{
	if (timeSlot % 4 == 0 || (radiosWithEmptyBand.size() == 0 && timeSlot % 2 == 0))
	{
		radiosWithEmptyBand.clear();
		channelSequence.empty();
		lowerBound1 = upperBound1 + 1;
		upperBound1 += RX.numberOfRadio;
		upperBound2 = lowerBound2 - 1;
		lowerBound2 -= RX.numberOfRadio;
		for (int b = lowerBound2; b <= upperBound2; b++)
		{
			channelSequence.push_back(b);
			channelSequence[RX.numberOfRadio + (b - lowerBound2)] = lowerBound1 + (b - lowerBound2);
		}
		for (int i = 0; i < RX.numberOfRadio; i++)
		{
			channelHoppingSequence[i] = channelSequence[i * 2];
			if (RX.scanningBands(Bands, channelHoppingSequence[i]))
				radiosWithEmptyBand.push_back(i);
			if (RX.scanningBands(Bands, channelHoppingSequence[i]))
				radiosWithEmptyBand.push_back(i);
			twoTimeSlotPassed[i] = true;
			if (RX.listening(Bands[channelHoppingSequence[i]], ID))
				return true;
		}
	}
	else
	{
		for (int i = 0; i < RX.numberOfRadio; i++)
		{
			if (std::find(radiosWithEmptyBand.begin(), radiosWithEmptyBand.end(), i) == radiosWithEmptyBand.end() || !twoTimeSlotPassed[i])
			{
				std::vector<int>::iterator itr = std::find(channelSequence.begin(), channelSequence.end(), channelHoppingSequence[i]);
				channelHoppingSequence[i] = channelSequence[std::distance(channelSequence.begin(), itr) + 1];
				if (RX.scanningBands(Bands, channelHoppingSequence[i]))
					radiosWithEmptyBand.push_back(i);
				else
				{
					radiosWithEmptyBand.erase(std::remove(radiosWithEmptyBand.begin(), radiosWithEmptyBand.end(), i), radiosWithEmptyBand.end());
				}
				twoTimeSlotPassed[i] = true;
				if (RX.listening(Bands[channelHoppingSequence[i]], ID))
					return true;
			}
			else if (twoTimeSlotPassed[i])
			{
				if (RX.scanningBands(Bands, channelHoppingSequence[i]))
					radiosWithEmptyBand.push_back(i);
				twoTimeSlotPassed[i] = false;
				if (RX.listening(Bands[channelHoppingSequence[i]], ID))
					return true;
			}
		}
	}
}
