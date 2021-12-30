function [rpp] = fxyz(a, F)
     xpp = 0.29*(sin(a(1))*sin(a(3)) + cos(a(1))*cos(a(3))*sin(a(2)))*(F(1) + F(2) + F(3) + F(4));
ypp = -0.29*(cos(a(3))*sin(a(1)) - 1.0*cos(a(1))*sin(a(2))*sin(a(3)))*(F(1) + F(2) + F(3) + F(4));
                        zpp = 0.29*cos(a(2))*cos(a(1))*(F(1) + F(2) + F(3) + F(4)) - 9.8;
     rpp = [xpp ypp zpp]';
end

