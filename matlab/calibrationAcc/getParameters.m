syms bx  by  bz  s c11 c12 c13 c21 c22 c23 c31 c32 c33 c41 c42 c43 real
%    150 200 100 20

%    0 0 10 =>  -150 -200 100
%    

k = 20;
bias = [10 20 1];

cases = [0 0 10;1 3 9.4868;-8 3 -5.1;2 7 -6.8557; 1 9.8995 1; 7 1.4142 -7; 2 3 -9.3274;6 6 5.2915; 1 1 9.8995; 9.8995 1 1 ];

eqs = [];
for i = 1:4
    
    c = zeros(3,1);
    for j = 1:3
        c(j) = (cases(i,j)*k-bias(j));
    end
    eqs = [eqs (s^2*100 == (c(1) + bx)^2 + (c(2) + by)^2 + (c(3) + bz)^2)]; 

end

% eqs = [(s^2*100 == (c11 + bx)^2 + (c12 + by)^2 + (c13 + bz)^2)
%         (s^2*100 == (c21 + bx)^2 + (c22 + by)^2 + (c23 + bz)^2)
%         (s^2*100 == (c31 + bx)^2 + (c32 + by)^2 + (c33 + bz)^2)
%         (s^2*100 == (c41 + bx)^2 + (c42 + by)^2 + (c43 + bz)^2)];
sol = vpasolve(eqs, [bx, by, bz, s]);
disp(sol.bx)
disp(sol.by)
disp(sol.bz)
disp(sol.s)


x2*x2-x1*x1+y2*y2-y1*y1+z2*z2-z1*z1 = 2*c1*(x1-x2)+2*c2*(y1-y2)+2*c3*(z1-z2)


m1 = n1*c1 + o1*c2 + p1*c3;
m2 = n2*c1 + o2*c2 + p2*c3;
m3 = n3*c1 + o3*c2 + p3*c3;

[ n1 n2 n3
  o1 o2 o3
  p1 p2 p3]

[c1
 c2
 c3]

c1 = 


