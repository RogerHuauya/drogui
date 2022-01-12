syms U1 U2 U3 rp pp yp rpp ypp ppp real

L = 0.4;
I =0.001*  [89.84704	0.00        0.00;
            0.000       94.36326	0.00;
            0.000       0.00        157.91544];

disp(vpa([rpp ppp ypp]' == I\([U1 U2 U3]' - cross([rp pp yp]',I*[rp, pp, yp]')),2))