syms U1 U2 U3 rp pp yp rpp ypp ppp real

L = 0.4;
I =[0.09566	0.000	0.00;
	0.000	0.09750	0.00;
	0.000	0.00	0.17147];

disp(vpa([rpp ppp ypp]' == I\([U1 U2 U3]' - cross([rp pp yp]',I*[rp, pp, yp]')),2))