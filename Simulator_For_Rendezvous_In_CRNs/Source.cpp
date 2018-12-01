#include "Initialization.h"
#include "Rendezvous_Algorithm.h"
#include"SecondaryUser.h"
int main()
{
	Initialization Start(100, 1000, 100, 0.4);
	Start.Initialize();
	bool rendezvous = false;
	Rendezvous_Algorithm tryToRendezvous;
	std::vector<SecondaryUser>::iterator RxConstruct;
	std::vector<SecondaryUser>::iterator TxConstruct;
	std::vector<int> successfulRendezvousChannels;
	while (!rendezvous)
	{
		for (int i = 0; i < 10 / 2; i++)
		{
			tryToRendezvous.ourAlgorithmTx();
			tryToRendezvous.ourAlgorithmRx();
		}
		/*Transmitter.ourAlgorithm(Start.arrivalBand, Start.SUs[0].emptyBands, 1);
		Receiver.ourAlgorithm(Start.arrivalBand, Start.SUs[1].emptyBands, 1);
		std::sort(Transmitter.channelHoppingSequence.begin(), Transmitter.channelHoppingSequence.end());
		std::sort(Receiver.channelHoppingSequence.begin(), Receiver.channelHoppingSequence.end());
		std::set_intersection(Transmitter.channelHoppingSequence.begin(), Transmitter.channelHoppingSequence.end(),
			Receiver.channelHoppingSequence.begin(), Receiver.channelHoppingSequence.end(), std::back_inserter(successfulRendezvousChannels));
		//successfulRendezvousChannels.resize(it - successfulRendezvousChannels.begin());
		//std::cout << successfulRendezvousChannels.size() << std::endl;
		if (successfulRendezvousChannels.size() > 0)
			rendezvous = true;
	}*/
	//std::cout << Transmitter.timeSlot << std::endl;
	system("pause");
}

