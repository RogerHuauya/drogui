function getFilterC(fc, fs)
    aux = 1:100;
    [~, d] = lowpass(aux, fc, fs,'ImpulseResponse','iir','Steepness',0.5);
    [b, a] = tf(d);
    [k, v] = tf2latc(b,a);
    disp(['k = {',sprintf('%d,', k(end:-1:2)), sprintf('%d', k(1)), '};'])
    disp(['v = {',sprintf('%d,', v(end:-1:2)), sprintf('%d', v(1)), '};'])
end