function plotFreq(a, Fs)
    L=length(a);
    Gx = fft(a);
    P2 = abs(Gx/L);
    P1 = P2(1:cast(L/2,'int64')+mod(L+1,2));
    P1(2:end-1) = 2*P1(2:end-1);
    f = Fs*(0:(L/2))/L;
    plot(f,P1) 
    title('Single-Sided Amplitude Spectrum of X(t)')
    xlabel('f (Hz)')
end