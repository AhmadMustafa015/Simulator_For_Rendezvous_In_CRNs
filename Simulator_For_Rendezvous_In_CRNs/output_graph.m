%MTTR
MTTRPU2 = csvread('Maximum Time to rendezvous for Vs SU_PUprob(.2).csv');
MTTRPU4 = csvread('Maximum Time to rendezvous for Vs SU_PUprob(.4).csv');
MTTRPU6 = csvread('Maximum Time to rendezvous for Vs SU_PUprob(.6).csv');

X1 = MTTRPU2(:,1);
X2 = MTTRPU4(:,1);
X3 = MTTRPU6(:,1);

figure(1);
hold on ;
          
 plot(X1,'b-o');
 plot(X2,'r--*');
 plot(X3,'g--^');
 title('MTTR Vs SU For differant PU activation probability');
 xlabel('SU');
 ylabel('MTTR');
 legend('PU prob (.2)','PU prob (.4)','PU prob (.6)');
 grid on;
 
MTTRR2 = csvread('Maximum Time to rendezvous for Vs SU_radio(1-5).csv');
MTTRR4 = csvread('Maximum Time to rendezvous for Vs SU_radio(2-4).csv');
MTTRR6 = csvread('Maximum Time to rendezvous for Vs SU_radio(3-3).csv');
X1 = MTTRR2(:,1);
X2 = MTTRR4(:,1);
X3 = MTTRR6(:,1);

figure(2);
hold on ;
          
 plot(X1,'b-o');
 plot(X2,'r--*');
 plot(X3,'g--^');
 grid on;
 title('MTTR Vs SU For differant number of radio');
 xlabel('SU');
 ylabel('MTTR');
 legend('number of radio (1-5)','number of radio (2-4)','number of radio (3-3');
 
 MTTRR22 = csvread('Maximum Time to rendezvous for Vs SU_radio(1-1).csv');
MTTRR44 = csvread('Maximum Time to rendezvous for Vs SU_radio(2-2).csv');
MTTRR66 = csvread('Maximum Time to rendezvous for Vs SU_radio(4-4).csv');
X1 = MTTRR22(:,1);
X2 = MTTRR44(:,1);
X3 = MTTRR66(:,1);

figure(3);
hold on ;
          
 plot(X1,'b-o');
 plot(X2,'r--*');
 plot(X3,'g--^');
 grid on;
 
 title('MTTR Vs SU For differant number of radio');
 xlabel('SU');
 ylabel('MTTR');
 legend('number of radio (1-1)','number of radio (2-2)','number of radio (4-4');
 
 %*******************************************************************************************
 TTRVsSU = csvread('Average TTR VS SU.csv');
 X1 = TTRVsSU(:,1);
  
 figure(4);
 hold on ;
 plot(X1, 'b-o');
 grid on;
  
 title('TTR Vs SU For Different PU Probability');
 xlabel('SU');
 ylabel('TTR');
 legend('PU Prob 0.2');
  
  
 