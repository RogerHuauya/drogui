% syms zeta a fc real
fc = 70;
wn = 2*pi*fc;
s = tf('s');
zeta = 10;
a = 1;
notch = (s^2+2*zeta*wn+wn^2)/(wn^2);
pole1 = (a*wn)/(s+a*wn);
pole2 = (wn/a)/(s+wn/a);
notch = notch*pole1*pole2;
Nz = c2d(notch,1/Fs,'tustin');
a_coeff = Nz.den{1};
b_coeff = Nz.num{1};
wfilt = filter(b_coeff,a_coeff,accel(:,1));
wfilt1 = filter(b_coeff1,a_coeff1,accel(:,1));