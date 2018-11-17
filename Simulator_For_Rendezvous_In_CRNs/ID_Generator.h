#pragma once
#include<vector>
#include<random>
#include<algorithm>
class ID_Generator
{
public:
	ID_Generator();
	~ID_Generator();
	void generatorIds(int numberOfSus, int IdLength);
	int getUserId(int suNumber) const;

private:
	std::vector<int> IDs;
};

