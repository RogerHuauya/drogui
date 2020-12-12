function signal_f = meanfilter(signal,windows)
    
    b = (1/windows)*ones(1,windows);
    a = 1;
    signal_f = filter(b,a,signal);
    
end

