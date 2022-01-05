function [app] = frpy(ap, U)
rpp = 10.0*U(1) - 0.77*ap(2)*ap(3);
ppp = 10.0*U(2) + 0.78*ap(1)*ap(3);
ypp = 5.8*U(3) - 0.011*ap(2)*ap(1);
app = [rpp ppp ypp]';
end

