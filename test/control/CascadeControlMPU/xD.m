close all
n = length(pitch);
dpitch = zeros(n,1);
dpitch(1) = pitch(1);
k = 0.0008;
disp(size(dpitch))
for i=2:n
    d = pitch(i) - dpitch(i-1);
    dpitch(i) = dpitch(i-1) + max(min(d, k), -k);
end
plot(pitch)
hold on
plot(dpitch)