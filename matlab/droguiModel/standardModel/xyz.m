syms r p y F1 F2 F3 F4 xpp ypp zpp real

m = 3.5;
g = 9.81;

disp(vpa([xpp ypp zpp]' == ([0 0 -m*g]' + rotZ(y)*rotY(p)*rotX(r)*[0 0 F1+F2+F3+F4]')/m,2))