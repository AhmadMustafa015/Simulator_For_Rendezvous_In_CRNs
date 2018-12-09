#pragma once
#include <iterator>
#include "Band_Details.h"
#include "SecondaryUser.h"
#include "Rendezvous_Algorithm.h"
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
	Initialization(int numOfBands, int numOfSUs, double PUProb,int timeSlot);
	~Initialization() {};
	void Initialize();
	int arrivalBand;
	std::vector<Transmitter> Tx;
	std::vector<Receiver> Rx;
	std::vector<Band_Details> Bands;
private:
	int numberOfBands;
	int timeSlot;
	int numberOfSUs;
	double PUProbON;
	bool rendezvous;
	void PUArrival(int arrivalBand, std::vector<Band_Details> &Bands);
	void intitialTransmittingAndReceiving(std::vector<Transmitter> &SUs, std::vector<Receiver> &SU,int numberOfBands);
	std::vector<Band_Details>::iterator bandConstruct;
	std::vector<SecondaryUser>::iterator SUsConstruct;
	std::vector<Rendezvous_Algorithm>::iterator RendezvouzTxIterator;
	std::vector<Rendezvous_Algorithm>::iterator RendezvouzRxIterator;
	std::vector<Rendezvous_Algorithm> channelHoppingTX;
	std::vector<Rendezvous_Algorithm> channelHoppingRX;
	int counter;
	std::vector<bool> successfulRendezvousVsSU;
};

