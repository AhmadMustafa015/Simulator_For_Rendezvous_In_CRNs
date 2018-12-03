#include "Rendezvous_Algorithm.h"
//std::random_device rand_dev5;
//std::mt19937 generator5(rand_dev5());

std::default_random_engine generator5(1);
Rendezvous_Algorithm::Rendezvous_Algorithm(int initialBand, Transmitter &Tx, std::vector<Band_Details> &Bands, int ID)
{
	distance = 2 * Tx.numberOfRadio;
	std::cout << "distance for TX ID " << ID << " = " << distance << " initial Band =  "<< initialBand << std::endl;
	upperBound1 = (initialBand + distance / 2) % Bands.size();
	lowerBound1 = (initialBand + 1) % Bands.size();
	upperBound2 = (initialBand - 1) % Bands.size();
	lowerBound2 = (initialBand - distance / 2) % Bands.size();
	channelHoppingSequence.resize(Tx.numberOfRadio);
	channelSequence.resize(distance);
	for (int b = lowerBound2; b <= upperBound2; b++)
	{
		channelSequence[b - lowerBound2] = b % Bands.size();
		channelSequence[Tx.numberOfRadio + (b - lowerBound2)] = (lowerBound1 + (b - lowerBound2)) % Bands.size();
	}
	for(int i = 0; i < channelSequence.size();i++)
		std::cout << channelSequence[i] << " ";
	std::cout << std::endl;
	for (int i = 0; i < Tx.numberOfRadio; i++)
	{
		channelHoppingSequence[i] = channelSequence[i * 2];
		if (Tx.scanningBands(Bands, channelHoppingSequence[i]))
			radiosWithEmptyBand.push_back(i);
	}
	for (int i = 0; i < channelHoppingSequence.size(); i++)
		std::cout << channelHoppingSequence[i] << " ";
	std::cout << std::endl;
	for (int i = 0; i < radiosWithEmptyBand.size(); i++)
		std::cout << radiosWithEmptyBand[i] << " ";
	std::cout << std::endl;
	if (!radiosWithEmptyBand.empty())
	{
		std::uniform_int_distribution<int> distr(1, radiosWithEmptyBand.size());
		if (radiosWithEmptyBand.size() != 0)
		{
			radioSendPacket = distr(generator5) - 1;
			Tx.sendPacket(Bands[channelHoppingSequence[radioSendPacket]], ID, radiosWithEmptyBand[radioSendPacket]);
		}
	}
	std::cout << "radio send Packet = " << radioSendPacket << std::endl;
	std::cout << "..................................................................................." << std::endl;
}
Rendezvous_Algorithm::Rendezvous_Algorithm(int initialBand, Receiver & RX, std::vector<Band_Details>& Bands, int ID)
	:twoTimeSlotPassed(RX.numberOfRadio , false)
{
	firstRendezvous = false;
	std::cout << "..................................................................................." << std::endl;
	distance = 2 * RX.numberOfRadio;
	std::cout << "distance for RX ID " << ID << " = " << distance << " initial Band =  " << initialBand << std::endl;
	upperBound1 = (initialBand + distance / 2) % Bands.size();
	lowerBound1 = (initialBand + 1) % Bands.size();
	upperBound2 = (initialBand - 1) % Bands.size();
	lowerBound2 = (initialBand - distance / 2) % Bands.size();
	channelHoppingSequence.resize(RX.numberOfRadio);
	channelSequence.resize(distance);
	for (int b = lowerBound2; b <= upperBound2; b++)
	{
		channelSequence[b - lowerBound2] = b % Bands.size();
		channelSequence[RX.numberOfRadio + (b - lowerBound2)] = (lowerBound1 + (b - lowerBound2)) % Bands.size();
	}
	for (int i = 0; i < RX.numberOfRadio; i++)
	{
		channelHoppingSequence[i] = channelSequence[i * 2];
		if (RX.scanningBands(Bands, channelHoppingSequence[i]))
			radiosWithEmptyBand.push_back(i);
		firstRendezvous =  RX.listening(Bands[channelHoppingSequence[i]], ID) + firstRendezvous;
		twoTimeSlotPassed[i] = true;
	}
	for (int i = 0; i < channelSequence.size(); i++)
		std::cout << channelSequence[i] << " ";
	std::cout << std::endl;
	for (int i = 0; i < channelHoppingSequence.size(); i++)
		std::cout << channelHoppingSequence[i] << " ";
	std::cout << std::endl;
	for (int i = 0; i < radiosWithEmptyBand.size(); i++)
		std::cout << radiosWithEmptyBand[i] << " ";
	std::cout << std::endl;
}
void Rendezvous_Algorithm::ourAlgorithmTx(int initialBand, Transmitter &Tx, std::vector<Band_Details> &Bands, int ID,int timeSlot)
{
	std::cout << "..................................................................................." << std::endl;
	std::cout << "TX ID =  " << ID << std::endl;
	//if(timeSlot == 4 * ) 
	if (timeSlot % 4 == 0 || (radiosWithEmptyBand.empty() && timeSlot % 2 == 0))
	{
		radiosWithEmptyBand.clear();
		channelSequence.clear();
		lowerBound1 = (upperBound1 + 1) % Bands.size();
		upperBound1 = (upperBound1 + Tx.numberOfRadio) % Bands.size();
		upperBound2 = (lowerBound2 - 1) % Bands.size();
		lowerBound2 = (lowerBound2 - Tx.numberOfRadio) % Bands.size();
		channelSequence.resize(distance);
		for (int b = lowerBound2; b <= upperBound2; b++)
		{
			channelSequence[b - lowerBound2] = b % Bands.size();
			channelSequence[Tx.numberOfRadio + (b - lowerBound2)] = (lowerBound1 + (b - lowerBound2)) % Bands.size();
		}
		for (int i = 0; i < Tx.numberOfRadio; i++)
		{
			channelHoppingSequence[i] = channelSequence[i * 2];
			if (Tx.scanningBands(Bands, channelHoppingSequence[i]))    
				radiosWithEmptyBand.push_back(i);
			//else
				//radiosWithEmptyBand.erase(std::remove(radiosWithEmptyBand.begin(), radiosWithEmptyBand.end(), i), radiosWithEmptyBand.end());
		}
		std::cout << "channel sequence : ";
		for (int i = 0; i < channelSequence.size(); i++)
			std::cout << channelSequence[i] << " ";
		std::cout << std::endl;
		std::cout << "channel Hopping sequence : ";
		for (int i = 0; i < channelHoppingSequence.size(); i++)
			std::cout << channelHoppingSequence[i] << " ";
		std::cout << std::endl;
	}
	else
	{
		for (int i = 0; i < Tx.numberOfRadio; i++)
		{
			if (radiosWithEmptyBand.empty() || std::find(radiosWithEmptyBand.begin(), radiosWithEmptyBand.end(), i) == radiosWithEmptyBand.end())
			{
				//++channelHoppingSequence[i];
				if (channelHoppingSequence[i] == upperBound1 || std::find(channelHoppingSequence.begin()
					, channelHoppingSequence.end(), (channelHoppingSequence[i] + 1)) != channelHoppingSequence.end())
				{
					for (int j = 0; j < Tx.numberOfRadio; j++)
					{
						if (find(channelHoppingSequence.begin(), channelHoppingSequence.end(),
							channelSequence[j * 2 + 1]) == channelHoppingSequence.end())
						{
							channelHoppingSequence[i] = channelSequence[j * 2 + 1];
							if (Tx.scanningBands(Bands, channelHoppingSequence[i]))
							{
								radiosWithEmptyBand.push_back(i);
								break;
							}
							else
								radiosWithEmptyBand.erase(std::remove(radiosWithEmptyBand.begin(), radiosWithEmptyBand.end(), i), radiosWithEmptyBand.end());
						}
					}
				}
				else
				{
					std::vector<int>::iterator itr = std::find(channelSequence.begin(), channelSequence.end(), channelHoppingSequence[i]);
					channelHoppingSequence[i] = channelSequence[std::distance(channelSequence.begin(), itr) + 1];
					if (Tx.scanningBands(Bands, channelHoppingSequence[i]))
						radiosWithEmptyBand.push_back(i);
					else
						radiosWithEmptyBand.erase(std::remove(radiosWithEmptyBand.begin(), radiosWithEmptyBand.end(), i), radiosWithEmptyBand.end());
				}
			}
		}
		std::cout << "Radio With empty Band : ";
		for (int i = 0; i < radiosWithEmptyBand.size(); i++)
			std::cout << radiosWithEmptyBand[i] << " ";
		std::cout << std::endl;
		std::cout << "channel hopping sequence : ";
		for (int i = 0; i < channelHoppingSequence.size(); i++)
			std::cout << channelHoppingSequence[i] << " ";
		std::cout << std::endl;
	}
	if (!radiosWithEmptyBand.empty())
	{
		std::uniform_int_distribution<int> distr(1, radiosWithEmptyBand.size());
		if (radiosWithEmptyBand.size() != 0)
		{
			radioSendPacket = distr(generator5) - 1;
			Tx.sendPacket(Bands[channelHoppingSequence[radioSendPacket]], ID, radiosWithEmptyBand[radioSendPacket]);
		}
		std::cout << "radio send Packet = " << radioSendPacket << std::endl;
	}
}

bool Rendezvous_Algorithm::ourAlgorithmRx(int initialBand, Receiver & RX, std::vector<Band_Details>& Bands, int ID, int timeSlot)
{
	if (timeSlot % 4 == 0 || (radiosWithEmptyBand.size() == 0 && timeSlot % 2 == 0))
	{
		radiosWithEmptyBand.clear();
		channelSequence.clear();
		lowerBound1 = (upperBound1 + 1) % Bands.size();
		upperBound1 = (upperBound1 + RX.numberOfRadio)  % Bands.size();
		upperBound2 = (lowerBound2 - 1) % Bands.size();
		lowerBound2 = (lowerBound2 - RX.numberOfRadio) % Bands.size();
		channelSequence.resize(distance);
		for (int b = lowerBound2; b <= upperBound2; b++)
		{
			channelSequence[b - lowerBound2] = b % Bands.size();
			channelSequence[RX.numberOfRadio + (b - lowerBound2)] = (lowerBound1 + (b - lowerBound2)) % Bands.size();
		}
		for (int i = 0; i < RX.numberOfRadio; i++)
		{
			channelHoppingSequence[i] = channelSequence[i * 2];
			if (RX.scanningBands(Bands, channelHoppingSequence[i]))
				radiosWithEmptyBand.push_back(i);
			//else
				//radiosWithEmptyBand.erase(std::remove(radiosWithEmptyBand.begin(), radiosWithEmptyBand.end(), i), radiosWithEmptyBand.end());
			twoTimeSlotPassed[i] = true;
			if (RX.listening(Bands[channelHoppingSequence[i]], ID))
				return true;
			return false;
		}
	}
	else
	{
		for (int i = 0; i < RX.numberOfRadio; i++)
		{
			if (std::find(radiosWithEmptyBand.begin(), radiosWithEmptyBand.end(), i) == radiosWithEmptyBand.end() || !twoTimeSlotPassed[i])
			{
				if (channelHoppingSequence[i] == upperBound1)
					channelHoppingSequence[i] = channelSequence[0];
				else
				{
					std::vector<int>::iterator itr = std::find(channelSequence.begin(), channelSequence.end(), channelHoppingSequence[i]);
					channelHoppingSequence[i] = channelSequence[std::distance(channelSequence.begin(), itr) + 1];
				}
				if (RX.scanningBands(Bands, channelHoppingSequence[i]))
					radiosWithEmptyBand.push_back(i);
				else
				{
					radiosWithEmptyBand.erase(std::remove(radiosWithEmptyBand.begin(), radiosWithEmptyBand.end(), i), radiosWithEmptyBand.end());
				}
				twoTimeSlotPassed[i] = true;
				if (RX.listening(Bands[channelHoppingSequence[i]], ID))
					return true;
				return false;
			}
			else if (twoTimeSlotPassed[i])
			{
				if (RX.scanningBands(Bands, channelHoppingSequence[i]))
					radiosWithEmptyBand.push_back(i);
				else
					radiosWithEmptyBand.erase(std::remove(radiosWithEmptyBand.begin(), radiosWithEmptyBand.end(), i), radiosWithEmptyBand.end());
				twoTimeSlotPassed[i] = false;
				if (RX.listening(Bands[channelHoppingSequence[i]], ID))
					return true;
				return false;

			}
		}
	}
}
