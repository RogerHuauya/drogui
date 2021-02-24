function [y_filt,y_ant,x_ant] = dynamicnotch(y_act,Fs,zeta,a,x_ant,y_ant,samples)
    L = samples;
    ffty = computefft(y_act,Fs);
    max_fft = 0;
    index = 0;
    x = (50*L/Fs);
    x = int64(x);
    for j = x:length(ffty) 
        if  ffty(j)> max_fft
            max_fft = ffty(j);
            index = j;
        end
    end
    fc = double(Fs*index/L);
    [num, den] = arr_notch(zeta, a, fc, Fs);
    den = den(2:end);
    x_ant(2:end) = x_ant(1:2);
    x_ant(1) = y_act(end);
    y = num * x_ant' - den*y_ant';
    y_ant(2) = y_ant(1);
    y_ant(1) = y;
    y_filt = y;
end

