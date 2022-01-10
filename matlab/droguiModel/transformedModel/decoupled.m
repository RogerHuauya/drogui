close all
syms U1 U2 U3 U4 u1 u2 u3 u4 M1 M2 M3 M4 F1 F2 F3 F4 real 
L = 0.4;
Kv = 0.05;
trottle = [50,65,75,85,100];
weight = 9.81/1000*[700,1060,1310,1620,1860];
[weight_trottle, S1] = polyfit( [0, weight], [0, trottle], 3);
[trottle_weight, S2] = polyfit( [0, trottle], [0, weight], 3);

% U = AF
A = [L -L -L L; -L -L L L; Kv*[-1 1 -1 1]; 1 1 1 1];

Ad = inv(A);
F = vpa(Ad*[U1 U2 U3 U4]',4)
Fvar = [F1 F2 F3 F4];
umotor = [];
for i=1:4
    umotor =[umotor poly2sym(sym(weight_trottle), Fvar(i))];
    %umotor(i) =  subs(umotor(i),  Fvar(i), F(i));
end

% umotor = simplify(umotor);
umotor = vpa(umotor, 5);
% 
% pretty(umotor);


