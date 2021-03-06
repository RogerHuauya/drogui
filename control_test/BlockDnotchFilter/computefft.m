function P1 = computefft(y,Fs)
    L = length(y);
    FFTY = fft(y);
    P2 = abs(FFTY/L);
%     disp("H");
%     disp(P2);
    P1 = P2(1:L/2+1);
    P1(2:end-1) = 2*P1(2:end-1);
%     disp(P1);
%     disp(length(P1))
end

