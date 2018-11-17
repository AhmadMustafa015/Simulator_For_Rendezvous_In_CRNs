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
	SecondaryUser(double PFA, double PMD, int NumberOfBand);	//Allow you to change the variable 
	~SecondaryUser() {};
	//void getBandNumber(int BN);							//get the number of bandwidth
	void scanningBands(const std::vector<Band_Details> &Bands, double asymettric);	//this function scan for empty bands and store them 															//in vector BandBeingScanned
	void emptyAllResult();
	std::vector<int> emptyBands;	// empty bands after sensing
	void selectUserId(const int &UserId);
private:
	int NumberOfBand;									
	unsigned int count = 0;
	 int numOfBandsReqForSUs; //Number of bands for SUs 5-15
	 int randomBand;
	 int UserID;
};

