close all 
Gr = 9.81;
Ts = 0.02;
N = length(ax);
p_gps = [x';y';z'];
p = zeros(3,N);
v = zeros(3,N);
q = zeros(4,N);


g = [0 0 1.02]'*Gr;
P = eye(9);
H = [eye(3) zeros(3,12)];
Q12 = eye(12)*0.001;
bias_u = zeros(6,1);
u = zeros(6,1);
R = eye(6)*0.01;
omg = zeros(4,4);
w = [wx wy wx];


s = [ax ay az]';

s_rot = zeros(3, N);
s_norm = zeros(3, N);
for i= 2:length(t_imu)
    Rq = rpy2R(roll(i), pitch(i), yaw(i) + pi/2);
    s_rot(:,i) = Rq*s(:,i);
    s_norm(:,i) = s_rot(:,i) + g/Gr;
    %s_rot(:,i) = Rq\s(:,i);
end
figure 
subplot(3, 1, 1);
scatter(t_imu, ax, 'r');
title("Aceleracion cruda");

subplot(3, 1, 2);
scatter(t_imu, ay, 'g');

subplot(3, 1, 3);
scatter(t_imu, az, 'b');


figure 
subplot(3, 1, 1);
scatter(t_imu, s_rot(1,:), 'r');
title("Aceleracion rotada");

subplot(3, 1, 2);
scatter(t_imu, s_rot(2,:), 'g');

subplot(3, 1, 3);
scatter(t_imu, s_rot(3,:), 'b');


lambda = 0.9;
j = 2;
s_filtered = zeros(3, length(t_imu));
s_filtered(3,1) = 1;
p_gps_ext = zeros(3, N);


for i= 2:length(t_imu)
    s_filtered(:,i) = s_filtered(:, i-1) + lambda*(s(:, i) - s_filtered(:, i-1));
end

figure 
subplot(3, 1, 1);
scatter(t_imu, s_filtered(1,:), 'r');
title("Aceleracion filtrada");

subplot(3, 1, 2);
scatter(t_imu, s_filtered(2,:), 'g');

subplot(3, 1, 3);
scatter(t_imu, s_filtered(3,:), 'b');


s_filtered = zeros(3, length(t_imu));
s_filtered(3,1) = -1;


for i= 2:length(t_imu)
    
    Rq = rpy2R(roll(i), pitch(i), yaw(i) - pi/2 + 0.5);
    
    omg(1,2) = w(:,3); 
    omg(3,4) = w(:,3);
    omg(1,3) = -w(:,2);
    omg(2,4) = w(:,2);
    omg(2,3) = w(:,1);
    omg(1,4) = w(:,1);
    omg = omg - omg';
    s_filtered(:,i) = s_filtered(:, i-1) + lambda*(s(:, i) - s_filtered(:, i-1));
    u = s_filtered(:, i)*Gr + bias_u*0.0005;
    p(:, i) = p(:, i-1) + Ts*v(:, i-1) + Ts^2/2*(Rq*u + g);
    v(:, i) = v(:, i-1) + Ts*(Rq*u + g);
    q(:, i) = (cos(0.5*Ts*norm(w))*eye(4) + sin(0.5*Ts*norm(w))*omg)*q(:,i-1);
    F = eye(15);
    F(1:3, 4:6) = Ts*eye(3);
    F(4:6, 7:9) = Ts*skew(Rq*s_filtered(:,i));
    F(4:6, 10:12) = Ts*Rq;
    F(7:9, 13:15) = -Ts*Rq;

    G = zeros(15,12);
    G(4:6, 1:3) = Ts*Rq;
    G(7:9, 4:6) = Ts*Rq;
    G(10:12, 7:9) = eye(3);
    G(13:15, 10:12) = eye(3);
    
    %plot(p(1,i), p(2,i),'g')

    P = F*P*F' + G*Q12*G';
    
    if (j <= length(p_gps) && t_gps(j) == t_imu(i))
        K = P*H'/(H*P*H' + R);
        ye = p_gps(:, j) - p(:, i);
        p_gps_ext(:,i)= p_gps(:, j);
        j = j + 1;
        delta = [zeros(9,1); bias_u] + K*ye;
        bias_u = delta(10:15);
        P = (eye(15) - K*H)*P;
        p(:, i) = p(:, i) + delta(1:3);
        v(:, i) = v(:, i) + delta(4:6);
        q(:, i) = 
        
    else
        p_gps_ext(:,i) = p_gps_ext(:,i-1);
    end
    %pause(0.1);
end
figure
animated = 0;
if animated == 1
    for i=1:N
        hold on
        plot(p(1, 1:i), p(2, 1:i), 'r');
        plot(p_gps_ext(1,1:i), p_gps_ext(2,1:i),'g')
        hold off
        %axis([0 4*r 0 2*r]);
        drawnow;
    end
else
    hold on
    plot(p(1,:), p(2,:), 'r');
    plot(p_gps(1,:), p_gps(2,:), 'g');
    hold off
end
figure
hold on
quiver(p(1,:), p(2, :), v(1, :), v(2, :), 'b');
quiver(p(1,:), p(2, :), s_rot(1, :), s_rot(2, :), 'g');
plot(p_gps_ext(1,:), p_gps_ext(2, :), 'r');
hold off
figure 
subplot(2, 2, 1);
scatter(t_imu, p(1,:), 'r');
title("Posicion vs. Velocidad");

subplot(2, 2, 2);
scatter(t_imu, v(1,:), 'b');

subplot(2, 2, 3);
scatter(t_imu, p(2,:), 'r');

subplot(2, 2, 4);
scatter(t_imu, v(2,:), 'b');


figure 
subplot(2, 2, 1);
scatter(t_imu, p(1,:), 'r');
title("Posicion estimada");

subplot(2, 2, 2);
scatter(t_imu, p_gps_ext(1,:), 'b');
title("Posicion gps");

subplot(2, 2, 3);
scatter(t_imu, p(2,:), 'r');

subplot(2, 2, 4);
scatter(t_imu, p_gps_ext(2,:), 'b');
% figure 
% subplot(3, 1, 1);
% scatter(t, s_filtered(1,:), 'r');
% title("Aceleracion filtrada");
% 
% subplot(3, 1, 2);
% scatter(t, s_filtered(2,:), 'g');
% 
% subplot(3, 1, 3);
% scatter(t, s_filtered(3,:), 'b');

% for i=1:3
%      plot(p(i,:));
%      hold on
%      plot(v(i,:), '-');
% end


