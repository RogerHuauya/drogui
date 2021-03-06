function [y_filt,y_ant,x_ant,fc_ant] = dynamicnotchM(y_act,Fs,zeta,a,x_ant,y_ant,samples, block,fc_ant)
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
    
   if (block == 1 )
%     [num, den] = arr_notch(zeta, a, fc, Fs); 
       fc_ant = fc;
   end
    [num, den] = arr_notch(zeta, a, fc_ant, Fs); 
    den = den(2:end);
    y = num * x_ant' - den*y_ant'; 
    x_ant(2:end) = x_ant(1:2);
    x_ant(1) = y_act(end);
    y_ant(2) = y_ant(1);
    y_ant(1) = y;
    y_filt = y;
end

