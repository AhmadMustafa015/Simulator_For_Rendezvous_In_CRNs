#pragma once
#include <iterator>
#include "Band_Details.h"
#include "SecondaryUser.h"
#include "Rendezvous_Algorithm.h"
#include <fstream>
#include <algorithm>
#include <string>
#include <numeric>
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
	void PUArrival(int arrivalBand, std::vector<Band_Details> &Bands, bool State);
	void intitialTransmittingAndReceiving(std::vector<Transmitter> &SUs, std::vector<Receiver> &SU,int numberOfBands);
	std::vector<Band_Details>::iterator bandConstruct;
	std::vector<SecondaryUser>::iterator SUsConstruct;
	std::vector<Rendezvous_Algorithm>::iterator RendezvousTxIterator;
	std::vector<Rendezvous_Algorithm>::iterator RendezvousRxIterator;
	std::vector<Rendezvous_Algorithm> channelHoppingTX;
	std::vector<Rendezvous_Algorithm> channelHoppingRX;
	std::vector<int> BandsOccupiedBySUs;
	int counter;
	std::vector<bool> successfulRendezvousVsSU;
	int numberOfRendezvous;
	std::vector<int> avgTimeToRendezvous;
	std::vector<bool> sst;
	double avgT;
	std::vector<std::vector<int>> avgTToRPerSUs;
};

