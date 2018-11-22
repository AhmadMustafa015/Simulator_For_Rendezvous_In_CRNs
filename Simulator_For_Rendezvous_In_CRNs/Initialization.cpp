#include "Initialization.h"

std::random_device rand_dev0;
std::mt19937 generator0(rand_dev0());
Initialization::Initialization(int numOfBands, int TimeSlots, int numOfSUs,double PUProb)
	:SUs(numOfSUs), Bands(numOfBands)
{
	numberOfBands = numOfBands;
	timeSlots = TimeSlots;
	numberOfSUs = numOfSUs;
	PUProbON = PUProb;
	Bands.reserve(numberOfBands);
	SUs.reserve(numberOfSUs);
}

void Initialization::Initialize()
{
	
	for (bandConstruct = Bands.begin(); bandConstruct != Bands.end(); bandConstruct++)
	{
		*bandConstruct = Band_Details(PUProbON);
	}
	for (SUsConstruct = SUs.begin(); SUsConstruct != SUs.end(); SUsConstruct++)
	{
		SUsConstruct->scanningBands(Bands);
	}
	TransmittingAndReceiving(SUs, numberOfBands);
	arrivalBand = SUs[0].allocatedBand;
	PUArrival(arrivalBand, Bands);


}

void Initialization::TransmittingAndReceiving(std::vector<SecondaryUser> &SUs, int numberOfBands)
{
	bool connected = false;
	int randomBand;
	std::vector<int> allocatedBands;
	std::uniform_int_distribution<int> distr(0, numberOfBands);
	//std::cout << SUs.size();
	for (unsigned int i = 0; i < (SUs.size() / 2); i++)
	{
		while (!connected)
		{
			randomBand = distr(generator0);
			std::cout << randomBand << " " << std::endl;
			for (int j = 0; j < SUs[0].emptyBands.size(); j++)
				std::cout << SUs[0].emptyBands[j] << " ";
			if (std::find(SUs[i].emptyBands.begin(), SUs[i].emptyBands.end(), randomBand) != SUs[i].emptyBands.end()
				&& std::find(allocatedBands.begin(), allocatedBands.end(), randomBand) == allocatedBands.end())
			{
				connected = true;
				allocatedBands.push_back(randomBand);
			}
		}
		SUs[i].bandAllocation(randomBand);
		SUs[i + 1].bandAllocation(randomBand);
		connected = false;
	}
}

void Initialization::PUArrival(int arrivalBand, std::vector<Band_Details> &Bands)
{
	Bands[arrivalBand].toggleState();
}
