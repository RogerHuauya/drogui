function [rpp] = fxyz(a, U)
    xpp = 0.29*U(4)*(sin(a(1))*sin(a(3)) + cos(a(1))*cos(a(3))*sin(a(2)));
    ypp = -0.29*U(4)*(cos(a(3))*sin(a(1)) - 1.0*cos(a(1))*sin(a(2))*sin(a(3)));
    zpp = 0.29*U(4)*cos(a(2))*cos(a(1)) - 9.8;
    rpp = [xpp ypp zpp]';
end

