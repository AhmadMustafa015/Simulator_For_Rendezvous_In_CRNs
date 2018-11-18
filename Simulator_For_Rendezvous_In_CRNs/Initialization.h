#pragma once
#include <iterator>
#include "Band_Details.h"
#include "SecondaryUser.h"

class Initialization
{
public:
	Initialization() {
		int numberOfBands = 100;
		int timeSlots = 1000;
		int numberOfSUs = 100;
		double PUProbON = 0.3;
		std::vector<Band_Details> Bands(numberOfBands);
		std::vector<SecondaryUser> SUs(numberOfSUs);

	};
	Initialization(int numOfBands, int TimeSlots, int numOfSUs, double PUProb);
	~Initialization() {};
	void Initialize();

private:
	int numberOfBands;
	int timeSlots;
	int numberOfSUs;
	double PUProbON;
	bool rendezvous;
	void PUArrival(int arrivalBand, std::vector<Band_Details> &Bands);
	void TransmittingAndReceiving(std::vector<SecondaryUser> &SUs, int numberOfBands);
	std::vector<Band_Details> Bands;
	std::vector<Band_Details>::iterator bandConstruct;
	std::vector<SecondaryUser> SUs;
	std::vector<SecondaryUser>::iterator SUsConstruct;
};

