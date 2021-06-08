L=4278;
Fs = 100;
Gx = fft(gy_n);
P2 = abs(Gx/L);
P1 = P2(1:L/2+1);
P1(2:end-1) = 2*P1(2:end-1);
f = Fs*(0:(L/2))/L;
figure(1);
plot(f,P1') 
title('Single-Sided Amplitude Spectrum of X(t)')
xlabel('f (Hz)')
%%
Gy = fft(gy);
P2 = abs(Gy/L);
P1 = P2(1:L/2+1);
P1(2:end-1) = 2*P1(2:end-1);
f = Fs*(0:(L/2))/L;
figure(2);
plot(f,P1) 
title('Single-Sided Amplitude Spectrum of X(t)')
xlabel('f (Hz)')
ylabel('|P1(f)|')