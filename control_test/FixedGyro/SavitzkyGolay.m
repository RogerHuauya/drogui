function ret = SavitzkyGolay(a, n)
    
    coeffs = [];
    if n == 7
        coeffs = [   0.10714,   0.07143,    0.03571, 0,...
                    -0.03571,   -0.07143,   -0.10714];
    elseif n == 5
        coeffs = [0.2, 0.1, 0, -0.1, -0.2];
    end
    
    ret = zeros(n-1,1);
    
    arr = zeros(length(a)-n+1, 1);
    
    for i = 1:n
        arr = arr +  coeffs(i)*a((7-i+1):(end-(i-1)));
    end
    
    ret = [ret; arr];
end