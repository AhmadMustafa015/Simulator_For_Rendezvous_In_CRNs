#pragma once
#include <vector>
#include <cmath>
#include<random>
#include"Band_Details.h"
#include "SecondaryUser.h"
class Rendezvous_Algorithm
{
public:
	Rendezvous_Algorithm() {};
	Rendezvous_Algorithm(int initialBand, Transmitter &Tx, std::vector<Band_Details> &Bands, int ID);
	Rendezvous_Algorithm(int initialBand, Receiver &Rx, std::vector<Band_Details> &Bands, int ID,std::vector<int> &occBands);
	~Rendezvous_Algorithm() {};
	void ourAlgorithmTx(int initialBand,Transmitter &SUs, std::vector<Band_Details> &Bands,int Id,int timeSlot);
	bool ourAlgorithmRx(int initialBand,Receiver &SUs, std::vector<Band_Details> &Bands,int Id, int timeSlot , std::vector<Rendezvous_Algorithm> &TXs, std::vector<int> &occBands);
	std::vector<int> channelHoppingSequence;
	bool firstRendezvous;
	std::vector<int> randomStay;
	std::vector<int> numberOfStayCounter;
	int distance;

private:
	//our algorithm 
	int upperBound1;
	int upperBound2;
	int lowerBound1;
	int lowerBound2;
	bool rendezvous;
	int radioSendPacket;
	std::vector<int> radiosWithEmptyBand;
	std::vector<int> channelSequence;
	std::vector<bool> twoTimeSlotPassed;
	int p;
	int returnMaxValueInVector(const std::vector<int> &V) const;
	std::vector<int> specialBands;
	std::vector<int> iterative;

	void setSpecialBands(int B);
	void removeFromSpecialBand(int B);
	int RandomStayConst;
	bool didntFinishWholeBound;
	bool state = false;
	int specialBandSendingTimes;
	int radioThatSendPacket;
	bool logical2;
};

