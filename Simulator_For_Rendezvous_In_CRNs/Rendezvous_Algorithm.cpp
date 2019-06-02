#include "Rendezvous_Algorithm.h"
//std::random_device rand_dev5;
//std::mt19937 generator5(rand_dev5());

std::default_random_engine generator5(1);
std::uniform_int_distribution<int> distrRandomStay(2, 5);

Rendezvous_Algorithm::Rendezvous_Algorithm(int initialBand, Transmitter &Tx, std::vector<Band_Details> &Bands, int ID)
	:numberOfStayCounter(Tx.numberOfRadio, 0), randomStay(Tx.numberOfRadio, 0)
{
	radioThatSendPacket = 0;
	logical2 = false; //delete
	specialBandSendingTimes = 0;
	state = false;
	didntFinishWholeBound = true; //delete
	radioThatSendPacket = 0;
	p = 5; //max stay time
	distance = 2 * Tx.numberOfRadio; //range of the TX is double its number of radio
	std::cout << "Distance for TX ID " << ID << " = " << distance << ", initial band =  "<< initialBand << std::endl;
	upperBound1 = (int(Bands.size()) + (initialBand + distance / 2) % int(Bands.size())) % int(Bands.size()); //upper bound 1 right of the intial band(modulu of number of all bands)
	lowerBound1 = (int(Bands.size())+(initialBand + 1) % int(Bands.size()))% int(Bands.size()); //lower bound for right of the initial band (modulu of number of all bands)
	upperBound2 =(int(Bands.size()) + (initialBand - 1) % int(Bands.size()))% int(Bands.size()); //left of the initial band
	lowerBound2 =(int(Bands.size()) + (initialBand - distance / 2) % int(Bands.size())) % int(Bands.size()); //left of the initial band
	channelHoppingSequence.resize(Tx.numberOfRadio); //resize the vector to the length equal to number of radio
	channelSequence.resize(distance); // resize the vector that contain all the available channels for the user bounded by distance
	for (int b = 0, BandTemp = lowerBound2 ; b < Tx.numberOfRadio; b++,BandTemp++)
	{
		//fill in parallel
		channelSequence[b] = BandTemp % int(Bands.size()); //fill the vector with the available chnannel for this user
		channelSequence[Tx.numberOfRadio + b] = (lowerBound1 + b) % int(Bands.size()); //fill the vector with the available chnannel for this user
	}
	std::cout << "Channel sequence : ";
	for(int i = 0; i < channelSequence.size();i++)
		std::cout << channelSequence[i] << " ";
	std::cout << std::endl;
	for (int i = 0; i < Tx.numberOfRadio; i++)
	{
		channelHoppingSequence[i] = channelSequence[i * 2]; //each radio hop on the channel (equally likely)
		if (Tx.scanningBands(Bands, channelHoppingSequence[i])) //check if specific band is empty (no PU)
		{
			radiosWithEmptyBand.push_back(i); //fill this vector with empty band (no PU) ************************
			randomStay[i] = distrRandomStay(generator5); //each radio stay for random time in the band
			setSpecialBands(channelHoppingSequence[i]); //added this band to special bands ---------NEW----------
			numberOfStayCounter[i]++; //count time slot for each radio which stay in the band to match with radomStay vector
		}
	}
	std::cout << "Channel hopping sequence : ";
	for (int i = 0; i < channelHoppingSequence.size(); i++)
		std::cout << channelHoppingSequence[i] << " ";
	std::cout << std::endl;
	std::cout << std::endl << "Radios with empty band : ";
	for (int i = 0; i < radiosWithEmptyBand.size(); i++)
		std::cout << radiosWithEmptyBand[i] << " ";
	std::cout << std::endl;
	if (!radiosWithEmptyBand.empty()) //make sure there is empty band no PU before check the redezvous
	{
		std::uniform_int_distribution<int> distr(1, radiosWithEmptyBand.size());
		if (radiosWithEmptyBand.size() != 0)
		{
			radioSendPacket = distr(generator5) - 1; // just for vector index start with 0
			Tx.sendPacket(Bands[channelHoppingSequence[radiosWithEmptyBand[radioSendPacket]]], ID, radiosWithEmptyBand[radioSendPacket]); //each radio send one packet
			std::cout << "Radio " << radiosWithEmptyBand[radioSendPacket] << " sends a packet on band " 
				<< channelHoppingSequence[radiosWithEmptyBand[radioSendPacket]] << std::endl;
		}
	}

	std::cout << "..................................................................................." << std::endl;
}

Rendezvous_Algorithm::Rendezvous_Algorithm(int initialBand, Receiver & RX, std::vector<Band_Details>& Bands, int ID, std::vector<int> &occBands)
	:twoTimeSlotPassed(RX.numberOfRadio, false), numberOfStayCounter(RX.numberOfRadio, 0), randomStay(RX.numberOfRadio, 0)
{
	firstRendezvous = false;
	std::cout << "..................................................................................." << std::endl;
	distance = 2 * RX.numberOfRadio;//range of the RX is double its number of radio
	std::cout << "Distance for RX ID " << ID << " = " << distance << ", initial band =  " << initialBand << std::endl;
	upperBound1 = (int(Bands.size()) + (initialBand + distance / 2) % int(Bands.size())) % int(Bands.size());//upper bound 1 right of the intial band(modulu of number of all bands)
	lowerBound1 = (int(Bands.size()) + (initialBand + 1) % int(Bands.size())) % int(Bands.size());//lower bound for right of the initial band (modulu of number of all bands)
	upperBound2 = (int(Bands.size()) + (initialBand - 1) % int(Bands.size())) % int(Bands.size());//left of the initial band
	lowerBound2 = (int(Bands.size()) + (initialBand - distance / 2) % int(Bands.size())) % int(Bands.size());//left of the initial band
	
	channelHoppingSequence.resize(RX.numberOfRadio);//resize the vector to the length equal to number of radio
	channelSequence.resize(distance);// resize the vector that contain all the available channels for the user bounded by distance
	for (int b = 0, BandTemp = lowerBound2; b < RX.numberOfRadio; b++, BandTemp++)
	{
		channelSequence[b] = BandTemp % int(Bands.size()); //fill the vector with the available chnannel for this user
		channelSequence[RX.numberOfRadio + b] = (lowerBound1 + b) % int(Bands.size()); //fill the vector with the available chnannel for this user
	}
	for (int i = 0; i < RX.numberOfRadio; i++)
	{
		channelHoppingSequence[i] = channelSequence[i * 2];//each radio hop on the channel (equally likely)
		if (RX.scanningBands(Bands, channelHoppingSequence[i]))//check if specific band is empty (no PU)
		{
			radiosWithEmptyBand.push_back(i); //fill this vector with empty band (no PU)
			randomStay[i] = distrRandomStay(generator5);//each radio stay for random time in the band
			setSpecialBands(channelHoppingSequence[i]); //added this band to special bands ---------NEW----------
			numberOfStayCounter[i]++;//count time slot for each radio which stay in the band to match with radomStay vector
		}
		if (RX.listening(Bands[channelHoppingSequence[i]], ID)) //check if successfuly rendezvous
		{
			std::cout << "RADIO NUMBER : " << i << "  " << "successfully rendezvous FROM FIRST TIME " << std::endl;
			RX.allocatedBand = channelHoppingSequence[i]; //reserve this band to the RX
			RX.numberOfRendezvous++; //how many time this User successfuly rendezvous
			occBands.push_back(channelHoppingSequence[i]);
		}
		firstRendezvous =  RX.listening(Bands[channelHoppingSequence[i]], ID) + firstRendezvous; //to complement the status of first rendezvous
		twoTimeSlotPassed[i] = true;
	}
	std::cout << "Channel sequence : "  ;
	for (int i = 0; i < channelSequence.size(); i++)
		std::cout << channelSequence[i] << " ";
	std::cout << std::endl;
	std::cout << "Channel hopping  sequence : " ;
	for (int i = 0; i < channelHoppingSequence.size(); i++)
		std::cout << channelHoppingSequence[i] << " ";
	std::cout << std::endl << "Radios with empty band : ";
	for (int i = 0; i < radiosWithEmptyBand.size(); i++)
		std::cout << radiosWithEmptyBand[i] << " ";
	std::cout << std::endl;
}
void Rendezvous_Algorithm::ourAlgorithmTx(int initialBand, Transmitter &Tx, std::vector<Band_Details> &Bands, int ID, int timeSlot)
{
	std::cout << "..................................................................................." << std::endl;
	std::cout << "TX ID =  " << ID << std::endl;
	//if(timeSlot == 4 * ) 
	if (returnMaxValueInVector(numberOfStayCounter) > returnMaxValueInVector(randomStay)
		|| (radiosWithEmptyBand.empty() && !didntFinishWholeBound)) //if the radio which have the higher number of waiting time has finish OR
																	//sens the whole band and found that its occupied by PU
	{														  
		if (state || specialBands.empty())
		{
			state = false;
			didntFinishWholeBound = true;
			radiosWithEmptyBand.clear();
			channelSequence.clear();
			//change the upper and lower limit
			lowerBound1 = (int(Bands.size()) + (upperBound1 + 1) % int(Bands.size())) % int(Bands.size());
			upperBound1 = (int(Bands.size()) + (upperBound1 + Tx.numberOfRadio) % int(Bands.size())) % int(Bands.size());
			upperBound2 = (int(Bands.size()) + (lowerBound2 - 1) % int(Bands.size())) % int(Bands.size());
			lowerBound2 = (int(Bands.size()) + (lowerBound2 - Tx.numberOfRadio) % int(Bands.size())) % int(Bands.size());
			channelSequence.resize(distance);
			for (int b = 0, BandTemp = lowerBound2; b < Tx.numberOfRadio; b++, BandTemp++) 
			{
				channelSequence[b] = BandTemp % int(Bands.size()); //fill the vector with channel sequence within the lower limit
				channelSequence[Tx.numberOfRadio + b] = (lowerBound1 + b) % int(Bands.size()); //fill the vector with upper limit sequence
			}
			for (int i = 0; i < Tx.numberOfRadio; i++)
			{
				numberOfStayCounter[i] = 0;
				randomStay[i] = 0;
				channelHoppingSequence[i] = channelSequence[i * 2]; //each radio hob on the specific band
				if (Tx.scanningBands(Bands, channelHoppingSequence[i])) //if band is empty
				{
					randomStay[i] = distrRandomStay(generator5);
					radiosWithEmptyBand.push_back(i);
					setSpecialBands(channelHoppingSequence[i]); //added this band to special band or increase its rank
					numberOfStayCounter[i]++;
				}
				else
				{
					removeFromSpecialBand(channelHoppingSequence[i]); //if there is PU
				}
			}
			if (!radiosWithEmptyBand.empty())
			{
				std::uniform_int_distribution<int> distr(1, radiosWithEmptyBand.size());
				if (radiosWithEmptyBand.size() != 0)
				{
					radioSendPacket = distr(generator5) - 1;
					Tx.sendPacket(Bands[channelHoppingSequence[radiosWithEmptyBand[radioSendPacket]]], ID, radiosWithEmptyBand[radioSendPacket]); //one radio send packet
				}
				std::cout << "Radio " << radiosWithEmptyBand[radioSendPacket] << " sends a packet on band "
					<< channelHoppingSequence[radiosWithEmptyBand[radioSendPacket]] << std::endl;
			}
		}
		else //if special band not empty
		{
			for (int i = 0; i < Tx.numberOfRadio; i++)
			{
				if (specialBands.size() > i && specialBands.size() > 0) //if the number radio higher than special band size don't execute this
					channelHoppingSequence[i] = specialBands[i]; //set next hop is the special band i
				if (Tx.scanningBands(Bands, channelHoppingSequence[i])) //sensing the band 
				{
					
					//randomStay[i] = distrRandomStay(generator5); //set random stay **************************************
					if (std::find(radiosWithEmptyBand.begin(), radiosWithEmptyBand.end(), i) == radiosWithEmptyBand.end()) //added radio ID the vector if it's not in the vector
						radiosWithEmptyBand.push_back(i);
					setSpecialBands(channelHoppingSequence[i]);
				}
				else
				{
					radiosWithEmptyBand.erase(std::remove(radiosWithEmptyBand.begin(), radiosWithEmptyBand.end(), i), radiosWithEmptyBand.end()); //if PU in the band 
					removeFromSpecialBand(channelHoppingSequence[i]); //ALso remove from special band											//remove it from vector
				}
			}
			for (radioThatSendPacket; radioThatSendPacket < Tx.numberOfRadio; radioThatSendPacket++) //radios that going to send packets every time slot
			{
				if (std::find(radiosWithEmptyBand.begin(), radiosWithEmptyBand.end(), radioThatSendPacket) != radiosWithEmptyBand.end()) //make sure that the radio will send 
				{																														//send the packet in empty band
				//	std::uniform_int_distribution<int> distr(1, radiosWithEmptyBand.size());********************
					Tx.sendPacket(Bands[channelHoppingSequence[radioThatSendPacket]], ID, radioThatSendPacket);
					specialBandSendingTimes++;
					std::cout << "Radio " << radioThatSendPacket << " sends a packet on band " << channelHoppingSequence[radioThatSendPacket] << std::endl;
					break;
				}
				//********** added else
			}
			if (specialBandSendingTimes == Tx.numberOfRadio || logical2)
			{
				radioThatSendPacket++;
				logical2 = true; //make sure just the first radio send packet for R time slot
			}
			if (radioThatSendPacket == Tx.numberOfRadio)
			{
				radioThatSendPacket = 0;
				state = true;
				logical2 = false;
			}
		}
		std::cout << "Channel sequence : ";
		for (int i = 0; i < channelSequence.size(); i++)
			std::cout << channelSequence[i] << " ";
		std::cout << std::endl;
		std::cout << "Channel hopping sequence : ";
		for (int i = 0; i < channelHoppingSequence.size(); i++)
			std::cout << channelHoppingSequence[i] << " ";
		std::cout << std::endl;
	}
	else
	{
		for (int i = 0; i < Tx.numberOfRadio; i++)
		{
			if (radiosWithEmptyBand.empty() || std::find(radiosWithEmptyBand.begin(), radiosWithEmptyBand.end(), i) == radiosWithEmptyBand.end() || numberOfStayCounter[i] > randomStay[i])
			{
				//++channelHoppingSequence[i];
				if (channelHoppingSequence[i] == upperBound1 || std::find(channelHoppingSequence.begin()
					, channelHoppingSequence.end(), (channelHoppingSequence[i] + 1)) != channelHoppingSequence.end())
				{
					for (int j = 0; j < Tx.numberOfRadio; j++)
					{
						if (find(channelHoppingSequence.begin(), channelHoppingSequence.end(),
							channelSequence[j * 2 + 1]) == channelHoppingSequence.end())
						{
							channelHoppingSequence[i] = channelSequence[j * 2 + 1];
							if (Tx.scanningBands(Bands, channelHoppingSequence[i]))
							{
								randomStay[i] = distrRandomStay(generator5);
								radiosWithEmptyBand.push_back(i);
								setSpecialBands(channelHoppingSequence[i]);
								numberOfStayCounter[i]++;
								break;
							}
							else
							{
								didntFinishWholeBound = false;
								radiosWithEmptyBand.erase(std::remove(radiosWithEmptyBand.begin(), radiosWithEmptyBand.end(), i), radiosWithEmptyBand.end());
								removeFromSpecialBand(channelHoppingSequence[i]);
								randomStay[i] = 0;
								numberOfStayCounter[i] = 0;
								break;
							}
						}
					}
				}
				else
				{
					std::vector<int>::iterator itr = std::find(channelSequence.begin(), channelSequence.end(), channelHoppingSequence[i]);
					channelHoppingSequence[i] = channelSequence[std::distance(channelSequence.begin(), itr) + 1];
					if (Tx.scanningBands(Bands, channelHoppingSequence[i]))
					{
						randomStay[i] = distrRandomStay(generator5);
						radiosWithEmptyBand.push_back(i);
						setSpecialBands(channelHoppingSequence[i]);
						numberOfStayCounter[i]++;
					}
					else
					{
						didntFinishWholeBound = false * didntFinishWholeBound;
						radiosWithEmptyBand.erase(std::remove(radiosWithEmptyBand.begin(), radiosWithEmptyBand.end(), i), radiosWithEmptyBand.end());
						removeFromSpecialBand(channelHoppingSequence[i]);
						randomStay[i] = 0;
						numberOfStayCounter[i] = 0;
					}
				}
			}
			else
			{
				if (!Tx.scanningBands(Bands, channelHoppingSequence[i]))
				{
					randomStay[i] = 0;
					numberOfStayCounter[i] = 0;
					radiosWithEmptyBand.erase(std::remove(radiosWithEmptyBand.begin(), radiosWithEmptyBand.end(), i), radiosWithEmptyBand.end());
					removeFromSpecialBand(channelHoppingSequence[i]);
					didntFinishWholeBound = false;
				}
				else
				{
					numberOfStayCounter[i]++;
					setSpecialBands(channelHoppingSequence[i]);
				}
			}

		}
		std::cout << "Radios with empty band : ";
		for (int i = 0; i < radiosWithEmptyBand.size(); i++)
			std::cout << radiosWithEmptyBand[i] << " ";
		std::cout << std::endl;
		std::cout << "Channel hopping sequence : ";
		for (int i = 0; i < channelHoppingSequence.size(); i++)
			std::cout << channelHoppingSequence[i] << " ";
		std::cout << std::endl;
		if (!radiosWithEmptyBand.empty())
		{
			std::uniform_int_distribution<int> distr(1, radiosWithEmptyBand.size());
			if (radiosWithEmptyBand.size() != 0)
			{
				radioSendPacket = distr(generator5) - 1;
				Tx.sendPacket(Bands[channelHoppingSequence[radiosWithEmptyBand[radioSendPacket]]], ID, radiosWithEmptyBand[radioSendPacket]);
			}
			std::cout << "Radio " << radiosWithEmptyBand[radioSendPacket] << " sends a packet on band " << channelHoppingSequence[radiosWithEmptyBand[radioSendPacket]] << std::endl;
		}
	}
	
}

bool Rendezvous_Algorithm::ourAlgorithmRx(int initialBand, Receiver & RX, std::vector<Band_Details>& Bands, int ID, int timeSlot
	, std::vector<Rendezvous_Algorithm> &TXs, std::vector<int> &occBands)
{
	std::cout << "..................................................................................." << std::endl;
	std::cout << "RX ID " << ID << std::endl;
	if (returnMaxValueInVector(numberOfStayCounter) > returnMaxValueInVector(randomStay)
		|| (radiosWithEmptyBand.empty() && !didntFinishWholeBound)) //increase bound if most radio should 
	{														  //stay finish
		if (state || specialBands.empty())
		{
			state = false;
			didntFinishWholeBound = true;
			radiosWithEmptyBand.clear();
			channelSequence.clear();

			lowerBound1 = (int(Bands.size()) + (upperBound1 + 1) % int(Bands.size())) % int(Bands.size());
			upperBound1 = (int(Bands.size()) + (upperBound1 + RX.numberOfRadio) % int(Bands.size())) % int(Bands.size());
			upperBound2 = (int(Bands.size()) + (lowerBound2 - 1) % int(Bands.size())) % int(Bands.size());
			lowerBound2 = (int(Bands.size()) + (lowerBound2 - RX.numberOfRadio) % int(Bands.size())) % int(Bands.size());

			
			channelSequence.resize(distance);
			for (int b = 0, BandTemp = lowerBound2; b < RX.numberOfRadio; b++, BandTemp++)
			{
				channelSequence[b] = BandTemp % int(Bands.size());
				channelSequence[RX.numberOfRadio + b] = (lowerBound1 + b) % int(Bands.size());
			}
			for (int i = 0; i < RX.numberOfRadio; i++)
			{
				numberOfStayCounter[i] = 0;
				randomStay[i] = 0;
				channelHoppingSequence[i] = channelSequence[i * 2];
				if (RX.scanningBands(Bands, channelHoppingSequence[i]))
				{
					randomStay[i] = distrRandomStay(generator5);
					radiosWithEmptyBand.push_back(i);
					setSpecialBands(channelHoppingSequence[i]);
					numberOfStayCounter[i]++;
				}
				else
				{
					removeFromSpecialBand(channelHoppingSequence[i]); //if there is PU
				}
				//else
				//radiosWithEmptyBand.erase(std::remove(radiosWithEmptyBand.begin(), radiosWithEmptyBand.end(), i), radiosWithEmptyBand.end());
				if (RX.listening(Bands[channelHoppingSequence[i]], ID))
				{
					for (int r = 0; r < RX.numberOfRadio; r++)
					{
						randomStay[r] = 0;
						numberOfStayCounter[r] = 0;
					}
					radiosWithEmptyBand.clear();
					radioThatSendPacket = 0;
					RX.allocatedBand = channelHoppingSequence[i];
					TXs[ID].radiosWithEmptyBand.clear();
					TXs[ID].radioThatSendPacket = 0;
					std::cout << "RADIO NUMBER : " << i << "  successfully rendezvous FROM UPPER AND LOWER LIMIT at time slot " << timeSlot << std::endl;
					occBands.push_back(channelHoppingSequence[i]);
					return true;
				}
			}

		}
		else
		{
			for (int i = 0; i < RX.numberOfRadio; i++)
			{
				if (specialBands.size() > i && specialBands.size() > 0)
					channelHoppingSequence[i] = specialBands[i];
				if (RX.scanningBands(Bands, channelHoppingSequence[i]))
				{

					randomStay[i] = distrRandomStay(generator5);
					if (std::find(radiosWithEmptyBand.begin(), radiosWithEmptyBand.end(), i) == radiosWithEmptyBand.end())
						radiosWithEmptyBand.push_back(i);
					setSpecialBands(channelHoppingSequence[i]);
					
				}
				else
				{
					radiosWithEmptyBand.erase(std::remove(radiosWithEmptyBand.begin(), radiosWithEmptyBand.end(), i), radiosWithEmptyBand.end());
					removeFromSpecialBand(channelHoppingSequence[i]); //if there is PU
				}
			}
			for (int i = 0; i < RX.numberOfRadio; i++)
			{
				if (std::find(radiosWithEmptyBand.begin(), radiosWithEmptyBand.end(), i) != radiosWithEmptyBand.end())
				{
					if (RX.listening(Bands[channelHoppingSequence[i]], ID))
					{
						for (int r = 0; r < RX.numberOfRadio; r++)
						{
							randomStay[r] = 0;
							numberOfStayCounter[r] = 0;
						}
						radiosWithEmptyBand.clear();
						radioThatSendPacket = 0;
						RX.allocatedBand = channelHoppingSequence[i];
						std::cout << "RADIO NUMBER : " << i <<"  " << "successfully rendezvous FROM SPECIAL BAND at time slot " << timeSlot << std::endl;
						TXs[ID].radiosWithEmptyBand.clear();
						TXs[ID].radioThatSendPacket = 0;
						occBands.push_back(channelHoppingSequence[i]);
						return true;
					}
				}
			}
			specialBandSendingTimes++;
			/*if (specialBandSendingTimes == RX.numberOfRadio || logical2)
			{
				radioThatSendPacket++;
				logical2 = true;
			}*/
			if (specialBandSendingTimes == 2 * RX.numberOfRadio - 1)
			{
				radioThatSendPacket = 0;
				specialBandSendingTimes = 0;
				state = true;
			}
		}
		std::cout << "channel Hopping  sequence : ";
		for (int i = 0; i < channelHoppingSequence.size(); i++)
			std::cout << channelHoppingSequence[i] << " ";
		std::cout << std::endl << "radio with empty band : ";
		for (int i = 0; i < radiosWithEmptyBand.size(); i++)
			std::cout << radiosWithEmptyBand[i] << " ";
		std::cout << std::endl;
		std::cout << "Special Bands : ";
		for (int i = 0; i < specialBands.size(); i++)
			std::cout << specialBands[i] << " ";
		std::cout << std::endl;
	}
	else
	{
		for (int i = 0; i < RX.numberOfRadio; i++)
		{
			if (radiosWithEmptyBand.empty() || std::find(radiosWithEmptyBand.begin(), radiosWithEmptyBand.end(), i) == radiosWithEmptyBand.end() || numberOfStayCounter[i] > randomStay[i])
			{
				//++channelHoppingSequence[i];
				if (channelHoppingSequence[i] == upperBound1 || std::find(channelHoppingSequence.begin()
					, channelHoppingSequence.end(), (channelHoppingSequence[i] + 1)) != channelHoppingSequence.end())
				{
					for (int j = 0; j < RX.numberOfRadio; j++)
					{
						if (find(channelHoppingSequence.begin(), channelHoppingSequence.end(),
							channelSequence[j * 2 + 1]) == channelHoppingSequence.end())
						{
							channelHoppingSequence[i] = channelSequence[j * 2 + 1];
							if (RX.scanningBands(Bands, channelHoppingSequence[i]))
							{
								randomStay[i] = distrRandomStay(generator5);
								radiosWithEmptyBand.push_back(i);
								setSpecialBands(channelHoppingSequence[i]);
								numberOfStayCounter[i]++;
								if (RX.listening(Bands[channelHoppingSequence[i]], ID))
								{
									for (int r = 0; r < RX.numberOfRadio; r++)
									{
										randomStay[r] = 0;
										numberOfStayCounter[r] = 0;
									}
									for (int r = 0; r < TXs[ID].distance /2; r++)
									{
										TXs[ID].randomStay[r] = 0;
										TXs[ID].numberOfStayCounter[r] = 0;
									}
									radiosWithEmptyBand.clear();
									radioThatSendPacket = 0;
									RX.allocatedBand = channelHoppingSequence[i];
									TXs[ID].radiosWithEmptyBand.clear();
									TXs[ID].radioThatSendPacket = 0;
									std::cout << "RADIO NUMBER : " << i << "  successfully rendezvous FROM MOVING TO BAND NOT CONTAIN RADIO at time slot " << timeSlot << std::endl;
									occBands.push_back(channelHoppingSequence[i]);
									return true;
								}
								break;
							}
							else
							{
								radiosWithEmptyBand.erase(std::remove(radiosWithEmptyBand.begin(), radiosWithEmptyBand.end(), i), radiosWithEmptyBand.end());
								removeFromSpecialBand(channelHoppingSequence[i]);
								randomStay[i] = 0;
								numberOfStayCounter[i] = 0;
								didntFinishWholeBound = false;
							}
						}
					}
				}
				else
				{
					std::vector<int>::iterator itr = std::find(channelSequence.begin(), channelSequence.end(), channelHoppingSequence[i]);
					channelHoppingSequence[i] = channelSequence[std::distance(channelSequence.begin(), itr) + 1];
					if (RX.scanningBands(Bands, channelHoppingSequence[i]))
					{
						randomStay[i] = distrRandomStay(generator5);
						radiosWithEmptyBand.push_back(i);
						setSpecialBands(channelHoppingSequence[i]);
						numberOfStayCounter[i]++;
						if (RX.listening(Bands[channelHoppingSequence[i]], ID))
						{
							for (int r = 0; r < RX.numberOfRadio; r++)
							{
								randomStay[r] = 0;
								numberOfStayCounter[r] = 0;
							}
							for (int r = 0; r < TXs[ID].distance / 2; r++)
							{
								TXs[ID].randomStay[r] = 0;
								TXs[ID].numberOfStayCounter[r] = 0;
							}
							radiosWithEmptyBand.clear();
							TXs[ID].radiosWithEmptyBand.clear();
							TXs[ID].radioThatSendPacket = 0;
							radioThatSendPacket = 0;
							RX.allocatedBand = channelHoppingSequence[i];
							std::cout << "RADIO NUMBER : " << i << "  successfully rendezvous FROM MOVING TO NEXT BAND at time slot " << timeSlot << std::endl;
							occBands.push_back(channelHoppingSequence[i]);
							return true;
						}
					}
					else
					{
						radiosWithEmptyBand.erase(std::remove(radiosWithEmptyBand.begin(), radiosWithEmptyBand.end(), i), radiosWithEmptyBand.end());
						removeFromSpecialBand(channelHoppingSequence[i]);
						randomStay[i] = 0;
						numberOfStayCounter[i] = 0;
						didntFinishWholeBound = false;
					}
				}
			}
			else
			{
				if (!RX.scanningBands(Bands, channelHoppingSequence[i]))
				{
					randomStay[i] = 0;
					numberOfStayCounter[i] = 0;
					radiosWithEmptyBand.erase(std::remove(radiosWithEmptyBand.begin(), radiosWithEmptyBand.end(), i), radiosWithEmptyBand.end());
					removeFromSpecialBand(channelHoppingSequence[i]);
					didntFinishWholeBound = false;
				}
				else
				{
					if (RX.listening(Bands[channelHoppingSequence[i]], ID))
					{
						for (int r = 0; r < RX.numberOfRadio; r++)
						{
							randomStay[r] = 0;
							numberOfStayCounter[r] = 0;
						}
						for (int r = 0; r < TXs[ID].distance / 2; r++)
						{
							TXs[ID].randomStay[r] = 0;
							TXs[ID].numberOfStayCounter[r] = 0;
						}
						radiosWithEmptyBand.clear();
						radioThatSendPacket = 0;
						RX.allocatedBand = channelHoppingSequence[i];
						TXs[ID].radiosWithEmptyBand.clear();
						TXs[ID].radioThatSendPacket = 0;
						std::cout << "RADIO NUMBER : " << i << "  successfully rendezvous FROM STAYING IN THE BAND at time slot " << timeSlot << std::endl;
						//occBands.push_back(channelHoppingSequence[i]);
						return true;
					}
					setSpecialBands(channelHoppingSequence[i]);
					numberOfStayCounter[i]++;
				}
			}

		}
		std::cout << "Radio With empty Band : ";
		for (int i = 0; i < radiosWithEmptyBand.size(); i++)
			std::cout << radiosWithEmptyBand[i] << " ";
		std::cout << std::endl;
		std::cout << "channel hopping sequence : ";
		for (int i = 0; i < channelHoppingSequence.size(); i++)
			std::cout << channelHoppingSequence[i] << " ";
		std::cout << std::endl;
		return false;
	}
}

int Rendezvous_Algorithm::returnMaxValueInVector(const std::vector<int>& V) const
{
	int max = 0;
	for (int i = 0; i < V.size(); i++)
	{
		if (max < V[i])
			max = V[i];
	}
	return max;
}

void Rendezvous_Algorithm::setSpecialBands(int B)
{
	//inmportant note: iterative mean the rank of the band in the SB vector they are syncronized (same indexs)
	int firstOutOfOrder, location;
	int temp , temp2;
	if (std::find(specialBands.begin(), specialBands.end(), B) == specialBands.end()) //CHECK if the band B not in the SB vector
	{
		specialBands.push_back(B);
		iterative.push_back(1); //set the rank of the band to 1
	}
	else //if its already in SB vector increase its rank  
	{
		std::vector<int>::iterator itr = std::find(specialBands.begin(), specialBands.end(), B); //return the index of the the band in the SB vector
		++iterative[std::distance(specialBands.begin(), itr)]; //increace the rank of the band
	}
	/*for (firstOutOfOrder = 1; firstOutOfOrder < specialBands.size(); firstOutOfOrder++) //Binary sorting 
	{
		if (iterative[firstOutOfOrder] < iterative[firstOutOfOrder - 1])
		{
			temp = specialBands[firstOutOfOrder];
			temp2 = iterative[firstOutOfOrder];
			location = firstOutOfOrder;
			do
			{
				specialBands[location] = specialBands[location - 1];
				iterative[location] = iterative[location - 1];
				location--;
			} while (location > 0 && iterative[location - 1] > temp);
			specialBands[location] = temp;
			iterative[location] = temp2;
		}
	}*/
	for (firstOutOfOrder = 1; firstOutOfOrder < iterative.size(); firstOutOfOrder++) //insertion sorting from the higher rank to lower -------new-------
	{
		temp = specialBands[firstOutOfOrder];
		temp2 = iterative[firstOutOfOrder];
		location = firstOutOfOrder - 1;
		while ((location >= 0) && (temp2 > iterative[location])) //based on ranking
		{
			specialBands[location + 1] = specialBands[location];
			iterative[location + 1] = iterative[location];
			location--;
		}
		specialBands[location + 1] = temp;
		iterative[location + 1] = temp2;
	}
}
void Rendezvous_Algorithm::removeFromSpecialBand(int B)
{	
	for (int i = 0; i < specialBands.size(); i++)
	{
		if (std::find(specialBands.begin(), specialBands.end(), B) != specialBands.end())
		{
			specialBands.erase(specialBands.begin() + i);
			iterative.erase(iterative.begin() + i);
		}
	}
}