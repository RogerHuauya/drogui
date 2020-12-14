close all 
Gr = 9.81;
Ts = 0.02;
N = length(ax);
p_gps = [x';y';z'];
p = zeros(3,N);
v = zeros(3,N);
g = [0 0 1.02]'*Gr;
P = eye(9)*0.1;
H = [eye(3) zeros(3,6)];
Q12 = eye(6)*0.01;
bias_u = zeros(3,1);
u = zeros(3,1);
R = eye(3)*0.1;
delta = zeros(9,1);

s = [ax ay az]';

s_rot = zeros(3, N);
s_norm = zeros(3, N);
for i= 2:length(t_mm)
    Rq = rpy2R(roll(i), pitch(i), yaw(i) - pi/2);
    s_rot(:,i) = Rq*s(:,i);
    s_norm(:,i) = s_rot(:,i) + g/Gr;
    %s_rot(:,i) = Rq\s(:,i);
end
figure 
subplot(3, 1, 1);
scatter(t_mm, ax, 'r');
title("Aceleracion cruda");

subplot(3, 1, 2);
scatter(t_mm, ay, 'g');

subplot(3, 1, 3);
scatter(t_mm, az, 'b');


figure 
subplot(3, 1, 1);
scatter(t_mm, s_rot(1,:), 'r');
title("Aceleracion rotada");

subplot(3, 1, 2);
scatter(t_mm, s_rot(2,:), 'g');

subplot(3, 1, 3);
scatter(t_mm, s_rot(3,:), 'b');


lambda = 0.9;
j = 2;
s_filtered = zeros(3, length(t_mm));
s_filtered(3,1) = 1;
p_gps_ext = zeros(3, N);


for i= 2:length(t_mm)
    s_filtered(:,i) = s_filtered(:, i-1) + lambda*(s(:, i) - s_filtered(:, i-1));
end

figure 
subplot(3, 1, 1);
scatter(t_mm, s_filtered(1,:), 'r');
title("Aceleracion filtrada");

subplot(3, 1, 2);
scatter(t_mm, s_filtered(2,:), 'g');

subplot(3, 1, 3);
scatter(t_mm, s_filtered(3,:), 'b');


s_filtered = zeros(3, length(t_mm));
s_filtered(3,1) = -1;

aux = zeros(3,1);
cnt = 0; flag = 0;
for i= 2:length(t_mm)
    
    Rq = rpy2R(roll(i), pitch(i), yaw(i) - pi/2);

    s_filtered(:,i) = s_filtered(:, i-1) + lambda*(s(:, i) - s_filtered(:, i-1));
    u = s_filtered(:, i)*Gr;%+ bias_u*0.005;
    s_rot(:,i) = Rq*u + g;
    
    if(flag == 1)
        p(:, i) = p(:, i-1) + Ts*v(:, i-1) + Ts^2/2*(s_rot(:,i));
        v(:, i) = v(:, i-1) + Ts*(s_rot(:,i));
    end
    
    F = [eye(3) Ts*eye(3) zeros(3,3); zeros(3,3) eye(3) Ts*Rq; zeros(3,3) zeros(3,3) eye(3)];
    G = [zeros(3,3) zeros(3,3); Ts*Rq zeros(3,3); zeros(3,3) eye(3)];
    %plot(p(1,i), p(2,i),'g')

    P = F*P*F' + G*Q12*G';
    
    if (j <= length(p_gps) && t_gps(j) == t_mm(i))
        flag = 1;
        cnt = 0;
        K = P*H'/(H*P*H' + R);
        ye = p_gps(:, j) - p(:, i);
        p_gps_ext(:,i)= p_gps(:, j);
        j = j + 1;
        delta = [zeros(6,1); bias_u] + K*ye;
        aux = [aux ye];
        bias_u = delta(7:9);
        P = (eye(9) - K*H)*P;
        %plot(p_gps(1,:), p_gps(2,:),'b')
        v(:, i) = v(:, i) + delta(4:6)/1.3;
    else
        p_gps_ext(:,i) = p_gps_ext(:,i-1);
    end
    
    if(cnt < 40)
        p(:, i) = p(:, i) + delta(1:3)/40;
    end
    cnt = cnt + 1;
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
plot(p(1,:), p(2, :), 'r');
plot(p_gps_ext(1,:), p_gps_ext(2, :), 'm');
hold off
figure 
subplot(2, 2, 1);
scatter(t_mm, p(1,:), 'r');
title("Posicion vs. Velocidad");

subplot(2, 2, 2);
scatter(t_mm, v(1,:), 'b');

subplot(2, 2, 3);
scatter(t_mm, p(2,:), 'r');

subplot(2, 2, 4);
scatter(t_mm, v(2,:), 'b');


figure 
subplot(2, 2, 1);
scatter(t_mm, p(1,:), 'r');
title("Posicion estimada");

subplot(2, 2, 2);
scatter(t_mm, p_gps_ext(1,:), 'b');
title("Posicion gps");

subplot(2, 2, 3);
scatter(t_mm, p(2,:), 'r');

subplot(2, 2, 4);
scatter(t_mm, p_gps_ext(2,:), 'b');
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


