close all;
Gr = 1;
Ts = 0.01;

N = length(Trpy.ACC_X);

shif = 0;
rng = (1:1:N);

ax = Trpy.ACC_X(rng)*Gr;
ay = Trpy.ACC_Y(rng)*Gr;
az = Trpy.ACC_Z(rng)*Gr;

roll = Trpy.RAW_ROLL(rng)*pi/180;
pitch = Trpy.RAW_PITCH(rng)*pi/180;
yaw = Trpy.RAW_YAW(rng)*pi/180;

roll_e = zeros(size(roll));
pitch_e = zeros(size(pitch));

N = length(rng);

ax_rot = zeros(size(Trpy.ACC_X(rng)));
ay_rot = zeros(size(Trpy.ACC_Y(rng)));
az_rot = zeros(size(Trpy.ACC_Z(rng)));

g = [0 0 9.81]';
P = eye(9)*1;
H = zeros(4,9); 
H(1:2,1:2) = eye(2); H(3:4,4:5) = eye(2);
Q = eye(6)*1;
R = eye(4)*1;

p = zeros(3,N);
v = zeros(3,N);
accR = zeros(3,N);

p_gps = [Txyz.GPS_X'; Txyz.GPS_Y'];
v_gps = [Txyz.GPS_VX'; Txyz.GPS_VY'];

bias_u = zeros(3,1);
deltas = zeros(9,1);
for i = 2:N

    Rq = rpy2R(roll(i), pitch(i), yaw(i));
    accR(:,i) = Rq'*[ax(i) ay(i) az(i)]' - g;
    
    p(:,i) = p(:,i-1) + Ts*v(:,i-1) + (Ts^2)/2*(accR(:,i));
    v(:,i) = v(:,i-1) + Ts*(accR(:,i));

    F = [    eye(3),  Ts*eye(3), zeros(3,3);
         zeros(3,3),     eye(3),      Ts*Rq;
         zeros(3,3), zeros(3,3),     eye(3); ];

    G = [   zeros(3,6);
            Ts*Rq zeros(3,3);
            zeros(3,3) eye(3); ];
    
    
    P = F*P*F' + G*Q*G';
    if (p_gps(1,i) ~= p_gps(1,i-1) || p_gps(2,i) ~= p_gps(2,i-1))

        K = P*H'/(H*P*H' + R);

        ye = [p_gps(:,i) - p(1:2,i); v_gps(:,i) - v(1:2,i)];

        deltas = [zeros(6,1); bias_u] + K*(ye);
        P = (eye(9) - K*H)*P;
        bias_u = deltas(7:9);
        p(1:3,i) = p(1:3,i) + deltas(1:3);
        v(1:3,i) = v(1:3,i) + deltas(4:6);

    end
    
end

figure
plotHold([p(1,:)', Txyz.GPS_X])

figure
plotHold([p(2,:)', Txyz.GPS_Y])

figure
plotHold([Txyz.X_VAL, Txyz.GPS_X])
legend('X VAL', 'GPS X')

figure
plotHold([Txyz.X_VAL, Txyz.GPS_Y])
legend('Y VAL', 'GPS Y')

% figure
% plotHold([v(1,:)', Txyz.XP_VAL])
% legend('Velx kalman posvel', 'Velx kalman pos' )
% 
% figure
% plotHold([v(2,:)', Txyz.YP_VAL])
% legend('Vely kalman posvel', 'Vely kalman pos' )

