#include "Initialization.h"

std::default_random_engine generator0(1);
//std::mt19937 generator0(rand_dev0());
Initialization::Initialization(int numOfBands, int numOfSUs, double PUProb, int timeSlots)
	:Tx(numOfSUs / 2), Bands(numOfBands), Rx(numOfSUs / 2), avgTToRPerSUs(numOfSUs / 2)
	, channelHoppingRX(numOfSUs / 2), channelHoppingTX(numOfSUs / 2)
	, successfulRendezvousVsSU(numOfSUs / 2, false)
{
	numberOfBands = numOfBands;
	numberOfSUs = numOfSUs;
	PUProbON = PUProb;
	Bands.reserve(numberOfBands);
	rendezvous = false;
	timeSlot = timeSlots;
	numberOfRendezvous = 0;
}

void Initialization::Initialize()
{
	counter = 0;
	for (bandConstruct = Bands.begin(); bandConstruct != Bands.end(); bandConstruct++)
	{
		*bandConstruct = Band_Details(PUProbON);
		if (Bands[counter].isEmpty())
			std::cout << counter << " ";
		counter++;
	}
	std::cout << std::endl << "----------------------------------------" << std::endl;
	/*for (SUsConstruct = SUs.begin(); SUsConstruct != SUs.end(); SUsConstruct++)
	{
		SUsConstruct->scanningBands(Bands);
	}*/
	intitialTransmittingAndReceiving(Tx, Rx, numberOfBands);
	counter = 0;
	for (RendezvousTxIterator = channelHoppingTX.begin(); RendezvousTxIterator != channelHoppingTX.end(); RendezvousTxIterator++)
	{
		*RendezvousTxIterator = Rendezvous_Algorithm(Tx[counter].allocatedBand, Tx[counter], Bands, counter);
		counter++;
	}
	counter = 0;
	for (RendezvousRxIterator = channelHoppingRX.begin(); RendezvousRxIterator != channelHoppingRX.end(); RendezvousRxIterator++)
	{
		*RendezvousRxIterator = Rendezvous_Algorithm(Rx[counter].allocatedBand, Rx[counter], Bands, counter);
		counter++;
	}
	for (int i = 0; i < numberOfSUs / 2; i++)
	{
		successfulRendezvousVsSU[i] = channelHoppingRX[i].firstRendezvous;
		if (channelHoppingRX[i].firstRendezvous)
			avgTimeToRendezvous.push_back(1);
		else
		{
			avgTimeToRendezvous.push_back(-1);
		}
		channelHoppingRX[i].firstRendezvous = false;
		sst.push_back(successfulRendezvousVsSU[i]);
	}

	for (int T = 1; T < timeSlot; T++)
	{
		if (T ==12)
			std::cout << "103";
		for (int i = 0; i < Bands.size(); i++)
		{
			if (Bands[i].isEmpty())
				Bands[i].setState((double(rand()) / double(RAND_MAX)) >= PUProbON);
			else
				Bands[i].setState((double(rand()) / double(RAND_MAX)) <= PUProbON);

			for (int j = 0; j < numberOfSUs / 2; j++)
			{
				if (!successfulRendezvousVsSU[j] && std::find(Bands[i].packetVsID.begin(), Bands[i].packetVsID.end(), j) != Bands[i].packetVsID.end())
					Bands[i].clearPacket();
				else if(!Bands[i].isEmpty())
				{
					Bands[i].clearPacket();
				}
			}
		}
	//	counter = 0;
		for (int i = 0; i < numberOfSUs / 2; i++)
			if (!successfulRendezvousVsSU[i])
				channelHoppingTX[i].ourAlgorithmTx(Tx[i].allocatedBand, Tx[i], Bands, i, T);
		for (int i = 0; i < numberOfSUs / 2; i++)
			if (!successfulRendezvousVsSU[i])
				successfulRendezvousVsSU[i] = channelHoppingRX[i].ourAlgorithmRx(Rx[i].allocatedBand, Rx[i], Bands, i, T);
		std::cout << "Rendezvous status: ";
		for (int i = 0; i < successfulRendezvousVsSU.size(); i++)
			std::cout << successfulRendezvousVsSU[i] << " ";
		std::cout << std::endl;

		for (int i = 0; i < numberOfSUs / 2; i++)
		{
			if (successfulRendezvousVsSU[i] && !Bands[Rx[i].allocatedBand].isEmpty())
			{
				std::cout << "PU come in band number " << Rx[i].allocatedBand << " " << std::endl;
				successfulRendezvousVsSU[i] = false;
				Bands[Rx[i].allocatedBand].clearPacket();
				RendezvousTxIterator = channelHoppingTX.begin() + i;
				*RendezvousTxIterator = Rendezvous_Algorithm(Rx[i].allocatedBand, Tx[i], Bands, i);
				RendezvousRxIterator = channelHoppingRX.begin() + i;
				*RendezvousRxIterator = Rendezvous_Algorithm(Rx[i].allocatedBand, Rx[i], Bands, i);
				successfulRendezvousVsSU[i] = channelHoppingRX[i].firstRendezvous;
			}
			if (channelHoppingRX[i].firstRendezvous)
				avgTimeToRendezvous.push_back(1);
			else
			{
				if (sst[i] != successfulRendezvousVsSU[i])
				{
					sst[i] = successfulRendezvousVsSU[i];
					if (successfulRendezvousVsSU[i])
						avgTimeToRendezvous.push_back(T);
					else if ((avgTimeToRendezvous.size() - numberOfSUs / 2) > 0 && avgTimeToRendezvous[avgTimeToRendezvous.size() - numberOfSUs / 2] >= 0)
					{
						avgTimeToRendezvous.push_back(-T);
					}

				}
				else
				{
					avgTimeToRendezvous.push_back(0);
				}
			}
			channelHoppingRX[i].firstRendezvous = false;
		}
		std::cout << std::endl << "////////////////////////////////////////////////////////////////" << T << std::endl;
	}

	for (int i = 0; i < numberOfSUs / 2; i++)
	{
		numberOfRendezvous += Rx[i].numberOfRendezvous;
	}
	std::cout << "***********************************      " << numberOfRendezvous << "          ***************************************" << std::endl;
	avgT = 0;
	for (int i = 0; i < avgTimeToRendezvous.size(); i++)
		avgT += avgTimeToRendezvous[i];
	avgT = avgT / avgTimeToRendezvous.size();
	std::cout << "***********************************      " << avgT <<"  " << avgTimeToRendezvous.size() << "          ***************************************" << std::endl;
	std::vector<int> numOfRProcess(numberOfSUs/2 , 0);
	for (int i = 0; i < numberOfSUs / 2; i++)
	{
		for (int k = 0; k < avgTimeToRendezvous.size() / (numberOfSUs / 2); k++)
		{
			avgTToRPerSUs[i].push_back(avgTimeToRendezvous[i + 5 * k]);
			if (avgTimeToRendezvous[i + 5 * k] < 0)
				numOfRProcess[i]++;
			if (avgTimeToRendezvous[i + 5 * k] == 1)
				numOfRProcess[i]++;
		}
		avgTToRPerSUs[i].push_back(numOfRProcess[i]);
		std::ofstream outputFile;
		outputFile.open("Time to rendezvous for SU ID(" + std::to_string(i) + ").csv");
		std::ostream_iterator<int> outputIterator(outputFile, "\n");
		std::copy(avgTToRPerSUs[i].begin(), avgTToRPerSUs[i].end(), outputIterator);
		outputFile.close();
	}

}


void Initialization::intitialTransmittingAndReceiving(std::vector<Transmitter> &Tx, std::vector<Receiver> &Rx, int numberOfBands)
{
	bool connected = false;
	int randomBand;
	std::vector<int> allocatedBands;
	std::uniform_int_distribution<int> distr(0, numberOfBands - 1);
	//std::cout << SUs.size();
	for (unsigned int i = 0; i < Tx.size(); i++)
	{
		while (!connected)
		{
			randomBand = distr(generator0);
			if (Bands[randomBand].isEmpty()
				&& std::find(allocatedBands.begin(), allocatedBands.end(), randomBand) == allocatedBands.end())
			{
				connected = true;
				allocatedBands.push_back(randomBand);
			}
		}
		Tx[i].bandAllocation(randomBand);
		Rx[i].bandAllocation(randomBand);
		PUArrival(randomBand, Bands, false);
		//arrivalBand = SUs[0].allocatedBand;
		connected = false;
	}
}

void Initialization::PUArrival(int arrivalBand, std::vector<Band_Details> &Bands, bool State)
{
	Bands[arrivalBand].setState(State);
}