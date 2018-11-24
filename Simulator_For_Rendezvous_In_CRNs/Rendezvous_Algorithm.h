#pragma once
#include<vector>
#include <cmath>
#include<random>
class Rendezvous_Algorithm
{
public:
	Rendezvous_Algorithm();
	~Rendezvous_Algorithm();
	void ourAlgorithm(int initialBand, std::vector<int> &availableBand, int numberOfRadios);
	std::vector<int> channelHoppingSequence;
	int timeSlot;

private:
	//our algorithm 
	int distance;
	int upperBound1;
	int upperBound2;
	int lowerBound1;
	int lowerBound2;
};

