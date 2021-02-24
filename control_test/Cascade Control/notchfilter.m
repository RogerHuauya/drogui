function x_filt=notchfilter(x,f,b)
    wo = f/(100/2);  
    bw = wo/b;
    [b,a] = iirnotch(wo,bw);
    x_filt = filter(b,a,x);
end