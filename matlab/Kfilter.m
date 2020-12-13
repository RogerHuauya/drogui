close all 
Gr = 9.81;
s = [ax_f';ay_f';az_f'];
Ts = 0.02;
N = 10000;
p_gps = [x';y';z'.*0];
p = zeros(3,1);
v = zeros(3,N);
g = [0 0 1]'*Gr;
P = eye(9)*0.1;
H = [eye(3) zeros(3,6)];
Q12 = eye(6)*0.25;
bias_u = zeros(3,1);
u = zeros(3,1);
R = eye(3)*0.25;

% N = length(ax);
% s = [ax ay az]';
% s_rot = zeros(3,N);
% for i= 2:length(t_imu)
%     Rq = rpy2R(roll(i), pitch(i), yaw(i));
%     s_rot(:,i) = Rq*s(:,i);
%     %s_rot(:,i) = Rq\s(:,i);
% end
% figure 
% subplot(3, 1, 1);
% scatter(t, ax, 'r');
% title("Aceleracion cruda");
% 
% subplot(3, 1, 2);
% scatter(t, ay, 'g');
% 
% subplot(3, 1, 3);
% scatter(t, az, 'b');
% 
% 
% figure 
% subplot(3, 1, 1);
% scatter(t, s_rot(1,:), 'r');
% title("Aceleracion rotada");
% 
% subplot(3, 1, 2);
% scatter(t, s_rot(2,:), 'g');
% 
% subplot(3, 1, 3);
% scatter(t, s_rot(3,:), 'b');
% 
% Ts = 0.02;
% p_gps = [x y z.*0]';
% p = zeros(3, N);
% v = zeros(3, N);
% g = [0 0 1.02]'*Gr;
% P = eye(9)*0.00001;
% H = [eye(3) zeros(3,6)];
% Q12 = eye(6)*0.0025;
% bias_u = zeros(3, 1);
% u = zeros(3,1);
% R = eye(3)*0.025;
ye = zeros(3,1);
lambda = 0.3;
j = 3;
s_filtered = zeros(3, length(t_imu));
s_filtered(3,1) = 1;
for i= 2:length(t_imu)
    
    Rq = rpy2R(roll(i), pitch(i), yaw(i));

    arz = [arz bias_u];
    %Rq = inv(Rq);
    
    s_filtered(:,i) = s_filtered(:, i-1) + lambda*(s(:, i) - s_filtered(:, i-1));
    u = s_filtered(:, i)*Gr;%+ bias_u;
    p(:, i) = p(:, i-1) + Ts*v(:, i-1) ;%+ Ts^2/2*(Rq*u + g);
    v(:, i) = v(:, i-1) + Ts*(Rq*u + g + bias_u);
    
    F = [eye(3) Ts*eye(3) zeros(3,3); zeros(3,3) eye(3) Ts*Rq; zeros(3,3) zeros(3,3) eye(3)];
    G = [zeros(3,3) zeros(3,3); Ts*Rq zeros(3,3); zeros(3,3) eye(3)];
    %plot(p(1,i), p(2,i),'g')

    P = F*P*F' + G*Q12*G';
    
    if (j <= 140 && t_gps(j) == t_imu(i))
        K = P*H'/(H*P*H' + R);
        ye = p_gps(:, j) - p(:, i);
        j = j + 1;
        delta = [zeros(6,1); bias_u] + K*ye;
        bias_u = delta(7:9);
        P = (eye(9) - K*H)*P;
        p(:, i) = p(:, i) + delta(1:3);
        %plot(p_gps(1,:), p_gps(2,:),'b')
        v(:, i) = v(:, i) + delta(4:6);
    end
    %pause(0.1);
end
figure
hold on
quiver(p(1,:), p(2,:), v(1,:), v(2,:));
plot(p_gps(1,:), p_gps(2,:),'g')
hold off

figure 
subplot(3, 1, 1);
scatter(t, s_filtered(1,:), 'r');
title("Aceleracion filtrada");

subplot(3, 1, 2);
scatter(t, s_filtered(2,:), 'g');

subplot(3, 1, 3);
scatter(t, s_filtered(3,:), 'b');

% for i=1:3
%      plot(p(i,:));
%      hold on
%      plot(v(i,:), '-');
% end


