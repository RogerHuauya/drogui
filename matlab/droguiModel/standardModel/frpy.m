function [app] = frpy(ap, F, M)
 rpp = 4.2*F(1) - 4.2*F(2) - 4.2*F(3) + 4.2*F(4) - 0.77*ap(2)*ap(3);
 ppp = 4.1*F(3) - 4.1*F(2) - 4.1*F(1) + 4.1*F(4) + 0.78*ap(1)*ap(3);
ypp = 5.8*M(2) - 5.8*M(1) - 5.8*M(3) + 5.8*M(4) - 0.011*ap(2)*ap(1);
app = [rpp ppp ypp]';
end

