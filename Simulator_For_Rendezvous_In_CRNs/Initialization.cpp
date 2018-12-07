#include "Initialization.h"

std::default_random_engine generator0(1);
//std::mt19937 generator0(rand_dev0());
Initialization::Initialization(int numOfBands, int numOfSUs,double PUProb)
	:Tx(numOfSUs/2), Bands(numOfBands), Rx(numOfSUs/2)
	,channelHoppingRX(numOfSUs/2),channelHoppingTX(numOfSUs/2)
	,successfulRendezvousVsSU(numOfSUs/2, false)
{
	timeSlot = 0;
	numberOfBands = numOfBands;
	numberOfSUs = numOfSUs;
	PUProbON = PUProb;
	Bands.reserve(numberOfBands);
	rendezvous = false;
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
	std::cout << std::endl;
	/*for (SUsConstruct = SUs.begin(); SUsConstruct != SUs.end(); SUsConstruct++)
	{
		SUsConstruct->scanningBands(Bands);
	}*/
	intitialTransmittingAndReceiving(Tx,Rx, numberOfBands);
	counter = 0;
	for (RendezvouzTxIterator = channelHoppingTX.begin(); RendezvouzTxIterator != channelHoppingTX.end(); RendezvouzTxIterator++)
	{
		*RendezvouzTxIterator = Rendezvous_Algorithm(Tx[counter].allocatedBand, Tx[counter], Bands, counter);
		counter++;
	}
	counter = 0;
	for (RendezvouzRxIterator = channelHoppingRX.begin(); RendezvouzRxIterator != channelHoppingRX.end(); RendezvouzRxIterator++)
	{
		*RendezvouzRxIterator = Rendezvous_Algorithm(Rx[counter].allocatedBand, Rx[counter], Bands, counter);
		counter++;
	}
	/*for (int i = 0; i < numberOfSUs / 2; i++)
	{
		successfulRendezvousVsSU[i] = channelHoppingRX[i].firstRendezvous;
	}*/
	for (int T = 0; T < timeSlot; T++)
	{
		for (int i = 0; i < Bands.size(); i++)
		{
			for (int j = 0; j < numberOfSUs / 2; j++)
			{
				if (!successfulRendezvousVsSU[j] && std::find(Bands[i].packetVsId.begin(), Bands[i].packetVsId.end(), j) != Bands[i].packetVsId.end())
					Bands[i].clearPacket();
			}
		}
		counter = 0;
		for (int i = 0; i < numberOfSUs / 2; i++)
			if (!successfulRendezvousVsSU[i])
				channelHoppingTX[i].ourAlgorithmTx(Tx[i].allocatedBand, Tx[i], Bands, i, T);
		for (int i = 0; i < numberOfSUs / 2; i++)
		{
			if (!successfulRendezvousVsSU[i])
				successfulRendezvousVsSU[i] = channelHoppingRX[i].ourAlgorithmRx(Rx[i].allocatedBand, Rx[i], Bands, i, T);
			counter += successfulRendezvousVsSU[i];
		}
		rendezvous = (numberOfSUs / 2) == (counter);
		for (int i = 0; i < successfulRendezvousVsSU.size(); i++)
			std::cout << successfulRendezvousVsSU[i] << " ";
		//std::cout << std::endl << "////////////////////////////////////////////////////////////////" << timeSlot << std::endl;
	}
	std::cout << "Successful Randezvous in : " << timeSlot << " Time slots" << std::endl;
		
}


void Initialization::intitialTransmittingAndReceiving(std::vector<Transmitter> &Tx, std::vector<Receiver> &Rx, int numberOfBands)
{
	bool connected = false;
	int randomBand;
	std::vector<int> allocatedBands;
	std::uniform_int_distribution<int> distr(0, numberOfBands-1);
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
		PUArrival(randomBand, Bands);
		//arrivalBand = SUs[0].allocatedBand;
		connected = false;
	}
}

void Initialization::PUArrival(int arrivalBand, std::vector<Band_Details> &Bands)
{
	Bands[arrivalBand].toggleState();
}
