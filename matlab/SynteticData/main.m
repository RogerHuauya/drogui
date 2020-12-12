close all 
Gr = 9.81;

Ts = 0.02;
N = 1000;
wn = 2*pi/40;
t = 0:Ts:Ts*(N-1);
r = 10;
x = r*(1 - cos(wn*t));
y = r*(sin(wn*t));
z = zeros(1, N);

vx = r*wn*sin(wn*t);
vy = r*wn*cos(wn*t);
ax = r*wn^2*cos(wn*t);
ay = -r*wn^2*(sin(wn*t));
az = zeros(1, N);
% for i=1:N
%     plot(x(1:i), y(1:i), 'r');
%     axis([0 2*r 0 r]);
%     drawnow;
% end
p_gps = [x ; y; z];

p = zeros(3,N);
v = [zeros(1, N); r*wn*zeros(1,N); zeros(1, N)];
g = [0 0 1]'*Gr;
P = eye(9)*0.1;
H = [eye(3) zeros(3,6)];
%Q12 = [10*ones(3,3) zeros(3,3); zeros(3,3) 400*ones(3,3)];
Q12 = eye(6,6)*100;
bias_u = zeros(3, N);
u = zeros(3,1);
R = eye(3)*1000;

s = [ax; ay; az];
w = wgn(3, N, 0)+4;
s = s + w;

s_rot = zeros(3,N);
ye = zeros(3,1);

for i=1:N
    s_rot(:, i) = Rq*s(:,i);
end

figure 
subplot(3, 1, 1);
scatter(t, ax, 'r');
title("Aceleracion cruda");

subplot(3, 1, 2);
scatter(t, ay, 'g');

subplot(3, 1, 3);
scatter(t, az, 'b');

figure 
subplot(3, 1, 1);
scatter(t, s_rot(1,:), 'r');
title("Aceleracion ruidosa");

subplot(3, 1, 2);
scatter(t, s_rot(2,:), 'g');

subplot(3, 1, 3);
scatter(t, s_rot(3,:), 'b');

lambda = 0.01;
s_filtered = zeros(3, N);
s_filtered(3,1) = 1;

for i= 2:N
    

    Rq = rpy2R(pi+wgn(1,1,-15),0 + wgn(1,1,-15), pi/3 + wgn(1,1,-15));

    s_filtered(:,i) = s_filtered(:, i-1) + lambda*(s(:, i) - s_filtered(:, i-1));
    u = s_filtered(:, i)+ bias_u(:,i-1);
    p(:, i) = p(:, i-1) + Ts*v(:, i-1)+ Ts^2/2*(Rq*u);
    v(:, i) = v(:, i-1) + Ts*(Rq*u);
    
    F = [eye(3) Ts*eye(3) zeros(3,3); zeros(3,3) eye(3) Ts*Rq; zeros(3,3) zeros(3,3) eye(3)];
    G = [zeros(3,3) zeros(3,3); Ts*Rq zeros(3,3); zeros(3,3) eye(3)];

    P = F*P*F' + G*Q12*G';
    
    if (mod(i, 50)==0)
        K = P*H'/(H*P*H' + R);
        ye = p_gps(:,i) - p(:, i);
        j = j + 1;
        delta = [zeros(6,1); bias_u(:,i-1)] + K*ye;
        bias_u(:,i) = delta(7:9);
        P = (eye(9) - K*H)*P;
        p(:, i) = p(:, i) + delta(1:3);
        v(:, i) = v(:, i) + delta(4:6);
    else
        bias_u(:,i) = bias_u(:,i-1);
    end
end
figure 
subplot(3, 1, 1);
scatter(t, bias_u(1,:), 'r');
title("bias u");

subplot(3, 1, 2);
scatter(t, bias_u(2,:), 'g');

subplot(3, 1, 3);
scatter(t, bias_u(3,:), 'b');

figure
% for i=1:N
%     hold on
%     plot(p(1, 1:i), p(2, 1:i), 'r');
%     plot(x(1:i), y(1:i), 'b');
%     hold off
%     axis([0 4*r 0 2*r]);
%     %drawnow;
% end
hold on
plot(p(1,:), p(2,:), 'r');
plot(x, y, 'b');
hold off
axis([0 2*r 0 r]);