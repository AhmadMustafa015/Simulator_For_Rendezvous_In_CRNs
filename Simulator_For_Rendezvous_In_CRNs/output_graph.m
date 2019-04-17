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
 axis([-inf inf 0 60])
 title('MTTR Vs SU for different PU activation probability');
 xlabel('SU');
 ylabel('MTTR');
 set(findall(gcf,'-property','FontSize'),'FontSize',13)
 legend('PU prob (0.2)','PU prob (0.4)','PU prob (0.75)');
 grid on;
 %*******************************************************************************************
MTTRR2 = csvread('Maximum Time to rendezvous for Vs SU_radio(1-5).csv');
MTTRR4 = csvread('Maximum Time to rendezvous for Vs SU_radio(1-4).csv');
MTTRR6 = csvread('Maximum Time to rendezvous for Vs SU_radio(2-4).csv');
X1 = MTTRR2(:,1);
X2 = MTTRR4(:,1);
X3 = MTTRR6(:,1);

figure(2);
hold on ;
          
 plot(X1,'b-o');
 plot(X2,'r--*');
 plot(X3,'g--^');
  axis([-inf inf 0 60])
 grid on;
 xlabel('SU');
 ylabel('MTTR');
 title('MTTR Vs SU For different number of radios at TX and RX');
 set(findall(gcf,'-property','FontSize'),'FontSize',13)
 legend('(1-5)', '(1-4)', '(2-4)');
 %*******************************************************************************************
MTTRR2 = csvread('Maximum Time to rendezvous for Vs SU_radio(5-1).csv');
MTTRR4 = csvread('Maximum Time to rendezvous for Vs SU_radio(4-1).csv');
MTTRR6 = csvread('Maximum Time to rendezvous for Vs SU_radio(4-2).csv');
X1 = MTTRR2(:,1);
X2 = MTTRR4(:,1);
X3 = MTTRR6(:,1);

figure(3);
hold on ;
          
 plot(X1,'b-o');
 plot(X2,'r--*');
 plot(X3,'g--^');
 axis([-inf inf 0 60])
 grid on;
 xlabel('SU');
 ylabel('MTTR');
 title('MTTR Vs SU For different number of radios at TX and RX');
 set(findall(gcf,'-property','FontSize'),'FontSize',13)
 legend('(5-1)', '(4-1)', '(4-2)');
 %*******************************************************************************************
 MTTRR22 = csvread('Maximum Time to rendezvous for Vs SU_radio(1-1).csv');
MTTRR44 = csvread('Maximum Time to rendezvous for Vs SU_radio(2-2).csv');
MTTRR66 = csvread('Maximum Time to rendezvous for Vs SU_radio(4-4).csv');
X1 = MTTRR22(:,1);
X2 = MTTRR44(:,1);
X3 = MTTRR66(:,1);

figure(4);
hold on ;
          
 plot(X1,'b-o');
 plot(X2,'r--*');
 plot(X3,'g--^');
 axis([-inf inf 0 60])
 grid on;
 
 xlabel('SU');
 ylabel('MTTR'); 
 title('MTTR Vs SU for equal number of radio at TX and RX');
 set(findall(gcf,'-property','FontSize'),'FontSize',13)
 legend('number of radio (1-1)','number of radio (2-2)','number of radio (4-4)');

 %*******************************************************************************************
 TTRVsSU15 = csvread('Average TTR VS SU(1, 5).csv');
 TTRVsSU24 = csvread('Average TTR VS SU(2, 4).csv');
 TTRVsSU33 = csvread('Average TTR VS SU(3, 3).csv');
 
 X1 = TTRVsSU15(:,1);
 X2 = TTRVsSU24(:,1);
 X3 = TTRVsSU33(:,1);
 
 figure(5);
 hold on ;
 plot(X1,'b-o');
 plot(X2,'r--*');
 plot(X3,'g--^');
 axis([-inf inf 0 10])
 grid on;
  
 title('TTR Vs SU For Different Radios');
 xlabel('SU');
 ylabel('TTR');
 set(findall(gcf,'-property','FontSize'),'FontSize',13)
 legend('(1, 5)', '(2, 4)', '(3, 3)');
 
 
 %*******************************************************************************************
 TTRVsSU51 = csvread('Average TTR VS SU(5, 1).csv');
 TTRVsSU42 = csvread('Average TTR VS SU(4, 2).csv');
 TTRVsSU33 = csvread('Average TTR VS SU(3, 3).csv');
 
 X1 = TTRVsSU51(:,1);
 X2 = TTRVsSU42(:,1);
 X3 = TTRVsSU33(:,1);
 
 figure(6);
 hold on ;
 plot(X1,'b-o');
 plot(X2,'r--*');
 plot(X3,'g--^');
 axis([-inf inf 0 10])
 grid on;
  
 title('TTR Vs SU For Different Radios');
 xlabel('SU');
 ylabel('TTR');
 set(findall(gcf,'-property','FontSize'),'FontSize',13)
 legend('(5, 1)', '(4, 2)', '(3, 3)');
 %*******************************************************************************************
 
 TTRVsSU11 = csvread('Average TTR VS SU(1, 1).csv');
 TTRVsSU22 = csvread('Average TTR VS SU(2, 2).csv');
 TTRVsSU55 = csvread('Average TTR VS SU(5, 5).csv');
 
 X1 = TTRVsSU11(:,1);
 X2 = TTRVsSU22(:,1);
 X3 = TTRVsSU55(:,1);
 
 figure(7);
 hold on ;
 plot(X1,'b-o');
 plot(X2,'r--*');
 plot(X3,'g--^');
 axis([-inf inf 0 10])
 grid on;
  
 title('TTR Vs SU For Same Radios');
 xlabel('SU');
 ylabel('TTR');
 set(findall(gcf,'-property','FontSize'),'FontSize',13)
 legend('(1, 1)', '(2, 2)', '(5, 5)');
 
 %*******************************************************************************************
 
UtilizationVSBand = csvread('Utilization VS Band.csv');
%  TTRVsSU24 = csvread('Average TTR VS SU(2, 4).csv');
%  TTRVsSU33 = csvread('Average TTR VS SU(3, 3).csv');
 
X1 = UtilizationVSBand(:,1);
%  X2 = TTRVsSU24(:,1);
%  X3 = TTRVsSU33(:,1);
 
figure(8);
hold on;
plot(X1,'b-o');
%  plot(X2,'r--*');
%  plot(X3,'g--^');
axis([-inf inf 0 1])
grid on;
  
title('Utilization VS Band');
xlabel('Band');
ylabel('Utilization');
% legend('(1, 5)', '(2, 4)', '(3, 3)');
  
set(findall(gcf,'-property','FontSize'),'FontSize',13)