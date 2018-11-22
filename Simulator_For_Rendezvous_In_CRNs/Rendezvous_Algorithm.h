#pragma once
#include<vector>
#include <cmath>
#include<random>
class Rendezvous_Algorithm
{
public:
	Rendezvous_Algorithm();
	~Rendezvous_Algorithm();
	bool ourAlgorithm(int initialBand, std::vector<int> &availableBand, int numberOfRadios);

private:
	//our algorithm 
	bool rendezvous;
	int distance;
	int upperBound1;
	int upperBound2;
	int lowerBound1;
	int lowerBound2;
	std::vector<int> rendezvousSequence;
	int timeSlot;
};

