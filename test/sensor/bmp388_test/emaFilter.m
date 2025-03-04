close all
N_bmp = 25;
a = 0.6;
b = 1 - a;
threshold = 0.5;
limits = 1;
press_r = press - mean(press);
atmospheric = press/100.0;
altitude = 44330.0 * (1.0 -  (atmospheric / atmospheric(1)).^0.1903);
aux = 0;
prom_altitude = zeros(1,length(altitude));
for i =N_bmp:length(press)
    prom_altitude(i) = mean(altitude(i-N_bmp+1:i));
end
%%
% prom_altitude = filter(b,a,prom_altitude);
y_out = zeros(1,length(prom_altitude));
for i = 1:(length(prom_altitude))
    u = y_out(i)*a + b*prom_altitude(i);
    diff = prom_altitude(i) - u;
    diff = max(min(diff, limits), -limits);
    if (abs(diff) > threshold) 
        u = u + diff/6.0;
    end
    y_out(i+1) = u;
end
aux = y_out;
figure;plot(y_out,'b');
%%
y_out = filter(b_f,a_f,y_out);
plot(altitude, 'b')
hold on
plot(y_out, 'r')
% figure
% plotFreq(y_out, 200)