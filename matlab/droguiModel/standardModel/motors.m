close all
trottle = [50,65,75,85,100];
weight = [700,1060,1310,1620,1860];


[weight_trottle, S] = polyfit( [0, weight], [0, trottle], 3);
test = polyval(weight_trottle, weight);
hold on
plot(weight, test)
plot(weight, trottle)
