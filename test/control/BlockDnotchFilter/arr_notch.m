function [b,a] = arr_notch(zeta,a_b,fc,fs)
wn = 2*pi*fc;
p = 2*zeta*wn + wn*wn;
ts = 1/fs;
q = wn^2;
n = wn*(a_b+1/a_b);
a = [1, (2*q*ts*ts-8)/(4+2*n*ts + q*ts*ts),(4-2*n*ts+q*ts*ts)/(4+2*n*ts + q*ts*ts)];
b = [(4+p*ts*ts)/(4+2*n*ts + q*ts*ts),(2*p*ts*ts-8)/(4+2*n*ts + q*ts*ts),(4+p*ts*ts)/(4+2*n*ts + q*ts*ts)];
end

