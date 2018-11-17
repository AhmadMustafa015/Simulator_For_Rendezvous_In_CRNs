#include "ID_Generator.h"


std::random_device rand_dev;
std::mt19937 generator(rand_dev());
ID_Generator::ID_Generator()
{
}


ID_Generator::~ID_Generator()
{
}

void ID_Generator::generatorIds(int SUsNumber ,int IdLength)
{
	std::uniform_int_distribution<int> distr(1, IdLength);
	int temp;
	for (int i = 0; i < SUsNumber; i++)
	{
		int temp = distr(generator);
		if(std::find(IDs.begin() , IDs.end() , temp) != IDs.end())
		IDs.push_back(distr(generator));
	}
}
int ID_Generator::getUserId(int suNumber) const
{
	return IDs[suNumber];
}