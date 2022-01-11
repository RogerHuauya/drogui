close all;
battery_weight = 1000;
% charge = 25000;
% battery_weight = 1000;
charge = 12500;
frame_weight = 2500;
drone_weight = battery_weight + frame_weight;


trottle = [50,65,75,85,100];
%weight = [1170,1530,1860,2300,2600];
%current = [7,10,13.4,17.6,21.3];
weight = [700,1060,1310,1620,1860];
current = [4.7,7.8,10.5,14,16.9];


weight_trottle = polyfit(weight,trottle,2);
weight_current = polyfit(weight,current,2);


payload = 0:100:5000;
total_weight = payload + drone_weight;
time = 60*((charge/1000.0)./(polyval(weight_current,total_weight/4)*4));
hover_trottle = polyval(weight_trottle,total_weight/4);

figure(1);

plot(payload, time);
hold on;
stem(payload, time);
% hold on;
% figure(2);
plot(payload, hover_trottle);
hold on;
stem(payload, hover_trottle);
hold on;
legend('Payload vs Timee',' Payload vs Hover Trottle ')

figure(2);

stem(hover_trottle, time,'b' );
hold on;
stem(hover_trottle, payload/100.0,'r');
legend('Hover Trottle vs Time',' Hover Trottle vs Payload/100 ')