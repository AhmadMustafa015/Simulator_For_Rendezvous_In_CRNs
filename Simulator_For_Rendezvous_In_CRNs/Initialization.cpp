#include "Initialization.h"

std::default_random_engine generator0(1);
//std::mt19937 generator0(rand_dev0());
Initialization::Initialization(int numOfBands, int numOfSUs, double PUProb, int timeSlots)
	:Tx(numOfSUs / 2), Bands(numOfBands), Rx(numOfSUs / 2), avgTToRPerSUs(numOfSUs / 2)
	, channelHoppingRX(numOfSUs / 2), channelHoppingTX(numOfSUs / 2)
	, successfulRendezvousVsSU(numOfSUs / 2, false), MTTRPerUser(numOfSUs / 1)
	, TTRVsSU(numOfSUs / 2, 0), utilizationVsBand(numOfBands, 0)
{
	numberOfBands = numOfBands;
	numberOfSUs = numOfSUs;
	PUProbON = PUProb;
	Bands.reserve(numberOfBands);
	rendezvous = false;
	timeSlot = timeSlots;
	numberOfRendezvous = 0;
}

void Initialization::Initialize()
{
	//std::cout.setstate(std::ios_base::failbit); //this line disable all cout 
	
	counter = 0;
	for (bandConstruct = Bands.begin(); bandConstruct != Bands.end(); bandConstruct++)
		//initilize all bands there number and weither there is PU or not
	{
		*bandConstruct = Band_Details(PUProbON);
		if (Bands[counter].isEmpty())
			std::cout << counter << " "; //print all empty bands
		counter++;
	}
	std::cout << std::endl << "----------------------------------------" << std::endl;
	intitialTransmittingAndReceiving(Tx, Rx, numberOfBands); //initilize that all pu disconnect because pu become active in there bands (handoff)
	counter = 0;
	for (RendezvousTxIterator = channelHoppingTX.begin(); RendezvousTxIterator != channelHoppingTX.end(); RendezvousTxIterator++)
	{
		*RendezvousTxIterator = Rendezvous_Algorithm(Tx[counter].allocatedBand, Tx[counter], Bands, counter); //apply our algorithm for the first time for TX
		counter++;
	}
	counter = 0;
	for (RendezvousRxIterator = channelHoppingRX.begin(); RendezvousRxIterator != channelHoppingRX.end(); RendezvousRxIterator++)
	{
		*RendezvousRxIterator = Rendezvous_Algorithm(Rx[counter].allocatedBand, Rx[counter], Bands, counter,occBands); //apply our algrithm for the first time for RX
		counter++;
	}
	std::cout << "Rendezvous status: ";
	for (int i = 0; i < numberOfSUs / 2; i++)
	{
		successfulRendezvousVsSU[i] = channelHoppingRX[i].firstRendezvous; //check if user i successfully rendezvous (TX with RX) 
		if (channelHoppingRX[i].firstRendezvous)
			avgTimeToRendezvous.push_back(1); //if they successfully rendezvous then add time to 1 
		else
		{
			avgTimeToRendezvous.push_back(-1); //else add -1 to indicate that they spend 1 time slote without rendezvous 
		}
		channelHoppingRX[i].firstRendezvous = false; //this variable use later in here we just initilize it to false for every RX
		sst.push_back(successfulRendezvousVsSU[i]); //this vector use for output
		std::cout << successfulRendezvousVsSU[i] << " "; //output if pair of users rendezvous or not
	}
	std::cout << std::endl << "//////////////////////  Time slot number ///////////////////////////// (" << 0 <<")" << std::endl;
	double probTest = 0;
	for (int T = 1; T < timeSlot; T++)
	{
		if (T ==9) //this if statment just for debugging
			std::cout << "103";
		for (int i = 0; i < Bands.size(); i++)
		{
			//below if and else for Markov chain 
			if (Bands[i].isEmpty())
				Bands[i].setState((double(rand()) / double(RAND_MAX)) >= 0.2); // Alpha 
			else
				Bands[i].setState((double(rand()) / double(RAND_MAX)) <= 0.3);  // Beta

			for (int j = 0; j < numberOfSUs / 2; j++)
			{
				if (!successfulRendezvousVsSU[j] && std::find(Bands[i].packetVsID.begin(), Bands[i].packetVsID.end(), j) != Bands[i].packetVsID.end())
					Bands[i].clearPacket(); //clear all packet in this band which is only one packet if the pu come (handoff)
				else if(!Bands[i].isEmpty())
				{
					Bands[i].clearPacket(); //or there is pu in this band (double check)
				}
			}
		}
		if (!Bands[10].isEmpty())
			probTest++;
	//	counter = 0;
		for (int i = 0; i < numberOfSUs / 2; i++)
			if (!successfulRendezvousVsSU[i])
				channelHoppingTX[i].ourAlgorithmTx(Tx[i].allocatedBand, Tx[i], Bands, i, T); //if the TX and RX didn't find each other so far do our algorithm fo TX
		for (int i = 0; i < numberOfSUs / 2; i++)
			if (!successfulRendezvousVsSU[i])
				successfulRendezvousVsSU[i] = channelHoppingRX[i].ourAlgorithmRx(Rx[i].allocatedBand, Rx[i], Bands, i, T , channelHoppingTX,occBands); //FOR RX
		std::cout << "Rendezvous status: ";
		for (int i = 0; i < successfulRendezvousVsSU.size(); i++)
			std::cout << successfulRendezvousVsSU[i] << " "; //print status of the SU 
		std::cout << std::endl;

		for (int i = 0; i < numberOfSUs / 2; i++)
		{
			if (successfulRendezvousVsSU[i] && !Bands[Rx[i].allocatedBand].isEmpty()) //if PU come to band that already SU in it
			{
				occBands.erase(std::remove(occBands.begin(), occBands.end(), Rx[i].allocatedBand), occBands.end()); //remove this band from occupeid band
				std::cout << "PU come in band number " << Rx[i].allocatedBand << " " << std::endl; //print the band number which pu come to it
				successfulRendezvousVsSU[i] = false; //update SU state
				Bands[Rx[i].allocatedBand].clearPacket(); //clear packet in the band
				RendezvousTxIterator = channelHoppingTX.begin() + i;
				*RendezvousTxIterator = Rendezvous_Algorithm(Rx[i].allocatedBand, Tx[i], Bands, i);
				RendezvousRxIterator = channelHoppingRX.begin() + i;
				*RendezvousRxIterator = Rendezvous_Algorithm(Rx[i].allocatedBand, Rx[i], Bands, i,occBands);
				successfulRendezvousVsSU[i] = channelHoppingRX[i].firstRendezvous;
			}
			if (channelHoppingRX[i].firstRendezvous)
				avgTimeToRendezvous.push_back(1);
			else
			{
				if (sst[i] != successfulRendezvousVsSU[i])
				{
					sst[i] = successfulRendezvousVsSU[i];
					if (successfulRendezvousVsSU[i])
						avgTimeToRendezvous.push_back(T);
					else if ((avgTimeToRendezvous.size() - numberOfSUs / 2) > 0 && avgTimeToRendezvous[avgTimeToRendezvous.size() - numberOfSUs / 2] >= 0)
					{
						avgTimeToRendezvous.push_back(-T);
					}

				}
				else
				{
					avgTimeToRendezvous.push_back(0);
				}
			}
			channelHoppingRX[i].firstRendezvous = false;
		}
		std::cout << std::endl << "//////////////////////  Time slot number ///////////////////////////// (" << T << ")" << std::endl;
		for (int k = 0; k < occBands.size(); k++)
		{
			++utilizationVsBand[occBands[k]];
		}
	}

	for (int i = 0; i < numberOfSUs / 2; i++)
	{
		numberOfRendezvous += Rx[i].numberOfRendezvous;
	}
	std::cout << "***********************************      " << numberOfRendezvous << "          ***************************************" << std::endl;
	avgT = 0;
	for (int i = 0; i < avgTimeToRendezvous.size(); i++)
		avgT += avgTimeToRendezvous[i];
	avgT = avgT / avgTimeToRendezvous.size();
	std::vector<int> numOfRProcess(numberOfSUs/2 , 0);
	int summ;
	std::cout.clear();
	for (int i = 0; i < numberOfSUs / 2; i++)
	{
		for (int k = 0; k < avgTimeToRendezvous.size() / (numberOfSUs / 2); k++)
		{
			avgTToRPerSUs[i].push_back(avgTimeToRendezvous[i + (numberOfSUs/2) * k]);
			if (avgTimeToRendezvous[i + 5 * k] < 0)
				numOfRProcess[i]++;
			if (avgTimeToRendezvous[i + 5 * k] == 1)
				numOfRProcess[i]++;
		}
		avgTToRPerSUs[i].push_back(numOfRProcess[i]);
		std::ofstream outputFile;
		outputFile.open("Time to rendezvous for SU ID(" + std::to_string(i) + ").csv");
		std::ostream_iterator<int> outputIterator(outputFile, "\n");
		std::copy(avgTToRPerSUs[i].begin(), avgTToRPerSUs[i].end(), outputIterator);
		outputFile.close();
		summ = std::accumulate(avgTToRPerSUs[i].begin(), avgTToRPerSUs[i].end(), 0);
		if (summ > 0)
			TTRVsSU[i] = double(summ / numOfRProcess[i]);
		else
		{
			TTRVsSU[i] = double((summ + LastFailedRendezvous(avgTToRPerSUs[i])) / numOfRProcess[i]);			// This gets rid of the last failed rendezvous
		}
		std::cout << "***********************************   For SU : " << i << "  " << double(summ/numOfRProcess[i])<< "          ***************************************" << std::endl;
		int tempr = 0;
		for(auto &t : avgTToRPerSUs[i])
		{
			if (t == avgTToRPerSUs[i].back())
				break;
			if (t < 0)
				tempr =t;
			if (t > 1)
			{
				tempr += t;
				MTTRPerUser[i].push_back(tempr);
			}
		}
		MTTRVsSU.push_back(maxValue(MTTRPerUser[i]));
	}
	std::ofstream outputFile;
	outputFile.open("Maximum Time to rendezvous for Vs SU.csv");
	std::ostream_iterator<int> outputIterator(outputFile, "\n");
	std::copy(MTTRVsSU.begin(), MTTRVsSU.end(), outputIterator);
	outputFile.close();

	std::ofstream outputFile0;
	outputFile0.open("Average TTR VS SU.csv");
	std::ostream_iterator<int> outputIterator0(outputFile0, "\n");
	std::copy(TTRVsSU.begin(), TTRVsSU.end(), outputIterator0);
	outputFile0.close();

	double ts = timeSlot;
	std::vector<double> oFile;
	std::transform(utilizationVsBand.begin(), utilizationVsBand.end(), std::back_inserter(oFile),
		[ts](int num) {return num / ts; });
	std::ofstream outputFile1;
	outputFile1.open("Utilization VS Band.csv");
	std::ostream_iterator<double> outputIterator1(outputFile1, "\n");
	std::copy(oFile.begin(), oFile.end(), outputIterator1);
	outputFile1.close();

	for (int i = 0; i < numberOfSUs / 2; i++)
		std::cout << successfulRendezvousVsSU[i] << "    ";
	std::cout << probTest / 100000.0;
}


void Initialization::intitialTransmittingAndReceiving(std::vector<Transmitter> &Tx, std::vector<Receiver> &Rx, int numberOfBands)
{
	bool connected = false;
	int randomBand;
	std::vector<int> allocatedBands;
	std::uniform_int_distribution<int> distr(0, numberOfBands - 1);
	for (unsigned int i = 0; i < Tx.size(); i++)
	{
		while (!connected)
		{
			randomBand = distr(generator0);
			if (Bands[randomBand].isEmpty()
				&& std::find(allocatedBands.begin(), allocatedBands.end(), randomBand) == allocatedBands.end())
			{
				connected = true;
				allocatedBands.push_back(randomBand);
			}
		}
		Tx[i].bandAllocation(randomBand);
		Rx[i].bandAllocation(randomBand);
		PUArrival(randomBand, Bands, false);
		connected = false;
	}
}

int Initialization::maxValue(const std::vector<int>& V)
{
	int max = 0;
	for (auto &t : V)
	{
		if (t > max)
			max = t;
	}
	return max;
}

int Initialization::LastFailedRendezvous(const std::vector<int>& V)
{
	for (int i = V.size() - 1; i >= 0; i--)
	{
		if (V[i] < 0)
			return -V[i];
	}
}

void Initialization::PUArrival(int arrivalBand, std::vector<Band_Details> &Bands, bool State)
{
	Bands[arrivalBand].setState(State);
}