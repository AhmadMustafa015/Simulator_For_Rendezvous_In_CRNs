#pragma once
#include<vector>
#include <cmath>
#include<random>
#include"Band_Details.h"
#include "SecondaryUser.h"
class Rendezvous_Algorithm
{
public:
	Rendezvous_Algorithm(int initialBand, Transmitter &Tx, std::vector<Band_Details> &Bands, int ID);
	~Rendezvous_Algorithm();
	void ourAlgorithmTx(int initialBand,Transmitter &SUs, std::vector<Band_Details> &Bands, int Id);
	void ourAlgorithmRx(int initialBand, Receiver &SUs, std::vector<Band_Details> &Bands, int Id);
	std::vector<int> channelHoppingSequence;
	int timeSlot;

private:
	//our algorithm 
	int distance;
	int upperBound1;
	int upperBound2;
	int lowerBound1;
	int lowerBound2;
	bool rendezvous;
	int radioSendPacket;
	std::vector<int> emptyBands;
};

