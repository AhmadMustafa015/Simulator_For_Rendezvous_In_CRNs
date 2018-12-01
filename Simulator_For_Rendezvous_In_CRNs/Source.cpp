#include "Initialization.h"
#include "Rendezvous_Algorithm.h"
int main()
{
	Initialization Start(100, 1000, 100, 0.4);
	Start.Initialize();
	bool rendezvous = false;
	Rendezvous_Algorithm Transmitter;
	Rendezvous_Algorithm Receiver;
	std::vector<int> successfulRendezvousChannels;
	while (!rendezvous)
	{
		Transmitter.ourAlgorithm(Start.arrivalBand, Start.SUs[0].emptyBands, 1);
		Receiver.ourAlgorithm(Start.arrivalBand, Start.SUs[1].emptyBands, 1);
		std::sort(Transmitter.channelHoppingSequence.begin(), Transmitter.channelHoppingSequence.end());
		std::sort(Receiver.channelHoppingSequence.begin(), Receiver.channelHoppingSequence.end());
		std::set_intersection(Transmitter.channelHoppingSequence.begin(), Transmitter.channelHoppingSequence.end(),
			Receiver.channelHoppingSequence.begin(), Receiver.channelHoppingSequence.end(), std::back_inserter(successfulRendezvousChannels));
		//successfulRendezvousChannels.resize(it - successfulRendezvousChannels.begin());
		std::cout << successfulRendezvousChannels.size() << std::endl;
		if (successfulRendezvousChannels.size() > 0)
			rendezvous = true;
	}
	std::cout << Transmitter.timeSlot << std::endl;
	system("pause");
}

