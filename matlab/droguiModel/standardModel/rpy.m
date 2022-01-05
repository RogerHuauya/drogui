syms F1 F2 F3 F4 M1 M2 M3 M4 rp pp yp rpp ypp ppp real

L = 0.4;
I =[0.09566	0.000	0.00;
	0.000	0.09750	0.00;
	0.000	0.00	0.17147];

disp(vpa([rpp ppp ypp]' == I\([L*(F1-F2-F3+F4) L*(-F1-F2+F3+F4) M2+M4-M1-M3]' - cross([rp pp yp]',I*[rp, pp, yp]')),2))