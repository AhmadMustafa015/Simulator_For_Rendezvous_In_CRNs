#include "Initialization.h"

std::random_device rand_dev0;
std::mt19937 generator0(rand_dev0());
Initialization::Initialization(int numOfBands, int TimeSlots, int numOfSUs,double PUProb)
	:Tx(numOfSUs/2), Bands(numOfBands), Rx(numberOfSUs/2)
{
	numberOfBands = numOfBands;
	timeSlots = TimeSlots;
	numberOfSUs = numOfSUs;
	PUProbON = PUProb;
	Bands.reserve(numberOfBands);
}

void Initialization::Initialize()
{
	
	for (bandConstruct = Bands.begin(); bandConstruct != Bands.end(); bandConstruct++)
	{
		*bandConstruct = Band_Details(PUProbON);
	}
	/*for (SUsConstruct = SUs.begin(); SUsConstruct != SUs.end(); SUsConstruct++)
	{
		SUsConstruct->scanningBands(Bands);
	}*/
	intitialTransmittingAndReceiving(Tx,Rx, numberOfBands);
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
