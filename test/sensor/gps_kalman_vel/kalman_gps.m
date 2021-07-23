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

z = Txyz.Z_VAL(rng);


roll_e = zeros(size(roll));
pitch_e = zeros(size(pitch));


vz = diff([0; Txyz.Z_VAL(rng)])/Ts;
N = length(rng);

ax_rot = zeros(size(Trpy.ACC_X(rng)));
ay_rot = zeros(size(Trpy.ACC_Y(rng)));
az_rot = zeros(size(Trpy.ACC_Z(rng)));


vx = zeros(1,N);
vy = zeros(1,N);

g = [0 0 9.81]';
P = eye(9)*1;
H = [eye(3) zeros(3,6)];
Q = eye(6)*1;
R = eye(3)*1;


p = zeros(3,N);
v = zeros(3,N);
accR = zeros(3,N);

p_gps = [Txyz.GPS_X'; Txyz.GPS_Y'; Txyz.Z_VAL'];

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

        ye = p_gps(:,i) - p(:,i);

        deltas = [zeros(6,1); bias_u] + K*(ye);
        P = (eye(9) - K*H)*P;
        bias_u = deltas(7:9);
        p(:,i) = p(:,i) + deltas(1:3);
        v(:,i) = v(:,i) + deltas(4:6);

    end
    
end

figure
plotHold([p(1,:)',p_gps(1,:)'])

figure
plotHold([ p(2,:)', p_gps(2,:)'])

% figure
% plotHold([ v(1,:)', ox*20])
% figure
% plotHold([ v(2,:)', oy*20])
% 
% figure
% plotHold([ v(1,:)', ox*20])
figure
plotHold([accR(1,:)', accR(2,:)', accR(3,:)'])
