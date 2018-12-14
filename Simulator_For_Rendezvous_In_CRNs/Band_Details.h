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
	void setState(bool PUState);
	~Band_Details();
	std::vector<int> packetVsID;
	void clearPacket();
	std::vector<int> occupants;
private:
	bool PUOFF;
	double ProbON;
	//std::vector<unsigned int> Occupants;
};
class DeterministicBand : public Band_Details
{
public:
	bool isEmpty(double T, int bandN) const;

private:

};