close all 
syms sx sy sz n ex ey ez Rq Ts real
syms p00 p01 p02 p03 p04 p05 p06 p07 p08  real
syms p10 p11 p12 p13 p14 p15 p16 p17 p18  real
syms p20 p21 p22 p23 p24 p25 p26 p27 p28  real
syms p30 p31 p32 p33 p34 p35 p36 p37 p38  real
syms p40 p41 p42 p43 p44 p45 p46 p47 p48  real
syms p50 p51 p52 p53 p54 p55 p56 p57 p58  real
syms p60 p61 p62 p63 p64 p65 p66 p67 p68  real
syms p70 p71 p72 p73 p74 p75 p76 p77 p78  real
syms p80 p81 p82 p83 p84 p85 p86 p87 p88  real

P =   [p00 p01 p02 p03 p04 p05 p06 p07 p08 ;
        p10 p11 p12 p13 p14 p15 p16 p17 p18 ;
        p20 p21 p22 p23 p24 p25 p26 p27 p28 ;
        p30 p31 p32 p33 p34 p35 p36 p37 p38 ;
        p40 p41 p42 p43 p44 p45 p46 p47 p48 ;
        p50 p51 p52 p53 p54 p55 p56 p57 p58 ;
        p60 p61 p62 p63 p64 p65 p66 p67 p68 ;
        p70 p71 p72 p73 p74 p75 p76 p77 p78 ;
        p80 p81 p82 p83 p84 p85 p86 p87 p88];

syms k00 k01 k02  real
syms k10 k11 k12  real
syms k20 k21 k22  real
syms k30 k31 k32  real
syms k40 k41 k42  real
syms k50 k51 k52  real
syms k60 k61 k62  real
syms k70 k71 k72  real
syms k80 k81 k82  real

K =   [k00 k01 k02 ;
        k10 k11 k12 ;
        k20 k21 k22 ;
        k30 k31 k32 ;
        k40 k41 k42 ;
        k50 k51 k52 ;
        k60 k61 k62 ;
        k70 k71 k72 ;
        k80 k81 k82 ];

clc

syms r00 r01 r02  real
syms r10 r11 r12  real
syms r20 r21 r22  real
Rq = [r00 r01 r02 ;
r10 r11 r12 ;
r20 r21 r22 ;
]
q = [n ex ey ez]';
G = 9.81;

s = [sx sy sz]'*G;

%Rq = quat2rotm(q');
% Rq(0+1, 0+1) = 2*(n*n + ex*ex) - 1;
% Rq(0+1, 1+1) = 2*(ex*ey - n*ez);
% Rq(0+1, 2+1) = 2*(ex*ez + n*ey);
% 
% Rq(1+1, 0+1) = 2*(ex*ey + n*ez);
% Rq(1+1, 1+1) = 2*(n*n + ey*ey) - 1;
% Rq(1+1, 2+1) = 2*(ey*ez - n*ex);
% 
% Rq(2+1, 0+1) = 2*(ex*ez - n*ey);
% Rq(2+1, 1+1) = 2*(ey*ez + n*ex);
% Rq(2+1, 2+1) = 2*(n*n + ez*ez) - 1;
anor = Rq*s;
%Ts = 0.01;
N = 10000;
p = zeros(3,N);
v = zeros(3,N);
g = [0 0 1]'*G;
F = [eye(3) Ts*eye(3) zeros(3,3); zeros(3,3) eye(3) Ts*Rq; zeros(3,3) zeros(3,3) eye(3)];
G = [zeros(3,3) zeros(3,3); Ts*Rq zeros(3,3); zeros(3,3) eye(3)];
%P = eye(9)*0.01;
H = [eye(3) zeros(3,6)];
Q12 = eye(6)*0.01;
bias_u = zeros(3,1);
u = zeros(3,1);
R = eye(3)*Ts;
ye = zeros(3,1);
Pnew = F*P*F' + G*Q12*G';
Knew = P*H'/(H*P*H' + R);
%Pgps = (eye(9) - K*H)*P;
% for i= 2:N
%     u = s + bias_u;
%     p(:,i) = p(:,i-1) + Ts*v(:,i-1) + Ts^2/2*(Rq*u + g);
%     v(:,i) = v(:,i-1) +Ts*(Rq*u + g);
%     P = F*P*F' + G*Q12*G';
%     if (mod(i,100) == 0)
%         K = P*H'/(H*P*H' + R);
%         ye = zeros(3,1) - p(:,i);
%         delta = [zeros(6,1); bias_u] + K*ye;
%         bias_u = delta(7:9);
%         %disp(bias_u)
%         disp(P*H')
%         P = (eye(9) - K*H)*P;
%         p(:,i) = p(:,i) + delta(1:3);
%         %disp(p(:,i))
%         v(:,i) = v(:,i) + delta(4:6);
%     end
% end
% for i=1:3
%     plot(p(i,:));
%     hold on
%     plot(v(i,:), '-');
% end
% hold off

