close all 
clc
q = [0.006 0.077 0.996 -0.007]';
G = 9.81;
s = [0.004 0.009 1.032]'*G;

Rq = quat2rotm(q');

anor = Rq*s;
Ts = 0.01;
N = 10000;
p = zeros(3,N);
v = zeros(3,N);
g = [0 0 1]'*G;
F = [eye(3) Ts*eye(3) zeros(3,3); zeros(3,3) eye(3) Ts*Rq; zeros(3,3) zeros(3,3) eye(3)];
G = [zeros(3,3) zeros(3,3); Ts*Rq zeros(3,3); zeros(3,3) eye(3)];
P = eye(9)*0.01;
H = [eye(3) zeros(3,6)];
Q12 = eye(6)*0.01;
bias_u = zeros(3,1);
u = zeros(3,1);
R = eye(3)*0.01;
ye = zeros(3,1);
for i= 2:N
    u = s + bias_u;
    p(:,i) = p(:,i-1) + Ts*v(:,i-1) + Ts^2/2*(Rq*u + g);
    v(:,i) = v(:,i-1) +Ts*(Rq*u + g);
    P = F*P*F' + G*Q12*G';
    if (mod(i,100) == 0)
        K = P*H'/(H*P*H' + R);
        ye = zeros(3,1) - p(:,i);
        delta = [zeros(6,1); bias_u] + K*ye;
        bias_u = delta(7:9);
        disp(bias_u)
        P = (eye(9) - K*H)*P;
        p(:,i) = p(:,i) + delta(1:3);
        %disp(p(:,i))
        v(:,i) = v(:,i) + delta(4:6);
    end
end
for i=1:3
    plot(p(i,:));
    hold on
    plot(v(i,:), '-');
end
hold off
