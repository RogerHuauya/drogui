function [k, v] = getFilter(fc, fs)
    aux = 1:100;
    [~, d] = lowpass(aux, fc, fs,'ImpulseResponse','iir','Steepness',0.5);
    [b, a] = tf(d);
    [k, v] = tf2latc(b,a);
end