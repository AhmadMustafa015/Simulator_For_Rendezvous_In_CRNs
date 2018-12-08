TTR = [1, 2 , 4 , 8 , 8 , 8 , 10 , 10]; %ppu = .2
NSUs = [10, 16 , 20, 26 , 30 , 40 , 46 , 50];

TTR2 = [2 , 3 , 8 , 11 , 11 , 11 , 11 , 11]; %ppu = .4
NSUs2 = [10 , 16 , 20 , 26 , 30 , 40, 46 , 50];

TTR3 = [5 , 12 , 12 , 14 , 14 , 16 , 20 , 32]; %ppu = .6
NSUs3 = [10 , 16 , 20 , 26 , 30 , 40, 46 , 50];
figure(1)
plot(NSUs , TTR ,'r-o' ,NSUs2 , TTR2 , 'm-^',NSUs3 , TTR3 ,'-x' )
grid on;
xlabel ('Number Of SUs');
ylabel ('TTR');
legend('TTR For probability Of PU is 0.2','TTR For probability Of PU is 0.4','TTR For probability Of PU is 0.6');
title('Time To Rendezvous Vs Number Of SUs');
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%5