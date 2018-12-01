#pragma once
#include <vector>
#include <iostream>
#include <random>
class Band_Details
{
public:
	Band_Details() {};
	Band_Details(double ProbON);
	bool isEmpty() const;
	bool getProbON() const;
	void toggleState();
	~Band_Details();
	std::vector<int> packetVsId;
	void clearPacket();
private:
	bool PUOFF;
	double ProbON;
	//std::vector<unsigned int> Occupants;
};
class DeterministicBand : public Band_Details
{
public:
	DeterministicBand(double, int);
	bool isEmpty(double T, int bandN) const;

private:
	int successfulVsTimePUActiveForBandN;
	double timeVSuccessfulReq;
};