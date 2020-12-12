function [f,Mag] = fourier(signal,fs)
    S = fft(signal);
    dim = size(S);
    L = dim(1);
    Mag = 2.*(abs(S(1:L/2)/L));
    
    f = fs.*(0:(L/2)-1)./L;
end

