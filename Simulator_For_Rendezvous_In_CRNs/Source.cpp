#include <iterator>
#include "Band_Details.h"
#include "SecondaryUser.h"
#include "ID_Generator.h"

int main()
{
	int numberOfBands = 100;
	int TimeSlot = 1000;
	int numberOfSUs = 100;
	double PUProbON = 0.3;
	double missDetection = 0;
	double falseAlarm = 0;
	int counter = 0;
	int IDLength = 10000;
	bool symmetric = true;
	double asymmetricity = 0.1;			// The higher the number, the higher the asymmetricity
	ID_Generator generateIdForUsers;
	generateIdForUsers.generatorIds(numberOfSUs, IDLength);
	std::vector<Band_Details> Bands(numberOfBands);
	Bands.reserve(numberOfBands);
	std::vector<Band_Details>::iterator bandConstruct;
	int i = 0;
	for (bandConstruct = Bands.begin(); bandConstruct != Bands.end(); bandConstruct++)
	{
		*bandConstruct = Band_Details(PUProbON);
		if (Bands[i].isEmpty() == false)
			counter++;
		++i;
	}
	std::cout << std::endl << counter << std::endl;
	std::vector<SecondaryUser> SUs(numberOfSUs);
	SUs.reserve(numberOfSUs);
	std::vector<SecondaryUser>::iterator SUsConstruct;
	int count = 0;
	for (SUsConstruct = SUs.begin(); SUsConstruct != SUs.end(); SUsConstruct++)
	{
		SUsConstruct->scanningBands(Bands, asymmetricity);
		SUsConstruct->selectUserId(generateIdForUsers.getUserId(count));
		count++;
	}
	for (int T = 0; T < TimeSlot; T++)
	{

	}
	system("pause");
}