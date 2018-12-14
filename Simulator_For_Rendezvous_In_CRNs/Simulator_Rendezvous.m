AvgTTRVSSU = csvread('Average TTR VS SU.csv');

grid on;
xlabel ('Number Of SUs');
ylabel ('TTR');
legend('TTR For probability Of PU is 0.2','TTR For probability Of PU is 0.4','TTR For probability Of PU is 0.6');
title('Time To Rendezvous Vs Number Of SUs');
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%5
