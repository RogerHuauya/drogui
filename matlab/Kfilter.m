close all 
Gr = 9.81;
s = [ax';ay';az'];
Ts = 0.02;
N = 10000;
p_gps = [x';y';z'.*0];
p = zeros(3,1);
v = zeros(3,N);
g = [0 0 1]'*Gr;
P = eye(9)*0.00001;
H = [eye(3) zeros(3,6)];
Q12 = eye(6)*0.0000025;
bias_u = zeros(3,1);
u = zeros(3,1);
R = eye(3)*0.0000025;
ye = zeros(3,1);

j = 1;
arz = zeros(3,1);
for i= 2:length(t_imu)
    
    Rq = rpy2R(roll(i), pitch(i), yaw(i));

    
    if det(Rq) < 0.5
        disp(Rq)
    end
    
    arz = [arz bias_u];
    %Rq = inv(Rq);
    
    %disp(Rq*u)
    u = s(:,i)*Gr + bias_u;
    p(:,i) = p(:,i-1) + Ts*v(:,i-1) + Ts^2/2*(Rq*u + g);
    v(:,i) = v(:,i-1) + Ts*(Rq*u + g);
    
    F = [eye(3) Ts*eye(3) zeros(3,3); zeros(3,3) eye(3) Ts*Rq; zeros(3,3) zeros(3,3) eye(3)];
    G = [zeros(3,3) zeros(3,3); Ts*Rq zeros(3,3); zeros(3,3) eye(3)];
       
    P = F*P*F' + G*Q12*G';
    if (j <= 140 && t_gps(j) == t_imu(i))
        %%disp("timer")
        K = P*H'/(H*P*H' + R);
        ye = p_gps(:,j) - p(:,i);
        j = j + 1;
        delta = [zeros(6,1); bias_u] + K*ye;
        bias_u = delta(7:9);
        %disp(bias_u)
        %disp(P*H')
        P = (eye(9) - K*H)*P;
        p(:,i) = p(:,i) + delta(1:3);
        %disp(p(:,i))
        v(:,i) = v(:,i) + delta(4:6);
    end
end

% for i=1:3
%      plot(p(i,:));
%      hold on
%      plot(v(i,:), '-');
% end

hold on
plot(p(1,:), p(2,:),'g')
plot(p_gps(1,:), p_gps(2,:),'b')

hold off
% 
figure(2)
hold on
subplot(3,1,1);plot(arz(1,:));
subplot(3,1,2);plot(arz(2,:));
subplot(3,1,3);plot(arz(3,:));

