function out = cleanData(in,threshold)
   [m,n] = size(in);
   out = zeros(m,n);
    for i = 2:m
        for j = 1:n
           if( abs(in(i,j) - in(i-1,j)) > threshold   )
               in(i,j) = in(i-1,j);
           end
           out(i,j) = in(i,j);
        end
   end
end