close all
trottle = [50,65,75,85,100];
weight = [700,1060,1310,1620,1860];


[weight_trottle, S] = polyfit([0, trottle],[0,weight],3);
test = polyval(weight_trottle,[0:50, trottle]);
hold on
plot([0:50, trottle], test)
plot(trottle, weight)
