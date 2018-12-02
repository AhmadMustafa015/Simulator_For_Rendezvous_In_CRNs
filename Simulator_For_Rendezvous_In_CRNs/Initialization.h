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
	int arrivalBand;
	std::vector<Transmitter> Tx;
	std::vector<Receiver> Rx;
	std::vector<Band_Details> Bands;
private:
	int numberOfBands;
	int timeSlots;
	int numberOfSUs;
	double PUProbON;
	bool rendezvous;
	void PUArrival(int arrivalBand, std::vector<Band_Details> &Bands);
	void intitialTransmittingAndReceiving(std::vector<Transmitter> &SUs, std::vector<Receiver> &SU,int numberOfBands);
	std::vector<Band_Details>::iterator bandConstruct;
	std::vector<SecondaryUser>::iterator SUsConstruct;
};

