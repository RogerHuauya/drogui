function f_filtered = filterSignal(signal,prototype,grade,wn)
   switch prototype
       case 0
           coef = fir1(grade,wn,'low');
           b = coef;
           a = 1;
       case 1
           	[b,a] = besself(grade,wn,'low');
       case 2
            [b,a] = butter(grade,wn,'low');
       case 3
            wp = 3;
            [b,a] = cheby1(grade,wp,wn,'low');
       otherwise
            [b,a] = cheby2(grade,40,wn,'low');
   end
       f_filtered = filter(b,a,signal); 
end