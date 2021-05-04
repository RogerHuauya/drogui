function [k, v] = getFilter(fc, fs, steep)
    aux = 1:100;
    [~, d] = lowpass(aux, fc, fs,'ImpulseResponse','iir','Steepness',steep);
    [b, a] = tf(d);
    [k, v] = tf2latc(b,a);
end