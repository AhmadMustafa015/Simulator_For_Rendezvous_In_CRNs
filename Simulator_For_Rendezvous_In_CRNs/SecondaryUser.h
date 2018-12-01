#pragma once
#include <vector>
#include "Band_Details.h"
#include <iostream>
#include <algorithm>
#include <iterator>
class SecondaryUser
{
public:
	SecondaryUser();
	~SecondaryUser() {};
	bool scanningBands(const std::vector<Band_Details> &Bands , int BandNumber);	//this function scan for empty bands and store them 
																					//in vector BandBeingScanned
	void bandAllocation(int randomBand);
	void emptyAllResult();
	std::vector<int> emptyBands;	// empty bands after sensing
	int numberOfRadio;

	int allocatedBand;

private:
	 int NumberOfBand;									
	 int randomBand;

};
class Transmitter :public SecondaryUser
{
public:
	Transmitter(int numberOfBand);
	void sendPacket(Band_Details &band, int ID, int radioNumber);
};
class Receiver :public SecondaryUser
{
	Receiver(int numberOfBand);
	void listening();
};

