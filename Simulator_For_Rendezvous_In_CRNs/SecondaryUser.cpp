#include "SecondaryUser.h"

//std::random_device rand_dev;
//std::mt19937 generator(rand_dev());
std::default_random_engine generator(1);
SecondaryUser::SecondaryUser()
{
	//NumberOfBand = 100;
	std::uniform_int_distribution<int> distr(1,1);
	numberOfRadio = distr(generator);
}
bool SecondaryUser::scanningBands(const std::vector<Band_Details> &Bands, int bandNumber)
{
	if (Bands[bandNumber].isEmpty())
	{
		emptyBands.push_back(bandNumber);
		return true;
	}
	else
		return false;
}
void SecondaryUser::bandAllocation(int randomBand)
{
	allocatedBand = randomBand;
}
void SecondaryUser::emptyAllResult() {
	emptyBands.clear();
}

void Transmitter::sendPacket(Band_Details &band,int ID, int radioNumber)
{
	band.packetVsId.push_back(ID);
}

bool Receiver::listening(Band_Details &band, int ID)
{
	if (band.packetVsId.size() == 1 && band.packetVsId[0] == ID)
	{
		//std::cout << std::endl <<"rendezvous" << ID;
		return true;
	}
	else
	{
		return false;
	}
}
