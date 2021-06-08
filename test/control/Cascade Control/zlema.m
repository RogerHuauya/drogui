function zle = zlema(a, alpha, gain)
zle = a;
for i = 2:length(zle)
    zle(i) = alpha*(zle(i)+gain*(zle(i)-zle(i-1)))+(1-alpha)*zle(i-1); 
end
end