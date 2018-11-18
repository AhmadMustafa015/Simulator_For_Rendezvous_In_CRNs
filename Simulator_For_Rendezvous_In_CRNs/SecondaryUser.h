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
	void scanningBands(const std::vector<Band_Details> &Bands);	//this function scan for empty bands and store them 
																					//in vector BandBeingScanned
	void bandAllocation(int randomBand);
	void emptyAllResult();
	std::vector<int> emptyBands;	// empty bands after sensing
	int allocatedBand;
private:
	 int NumberOfBand;									
	 int randomBand;

};

