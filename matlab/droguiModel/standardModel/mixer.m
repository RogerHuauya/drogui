function [u] = mixer(R,P,Y,H)
    M1 = H +R -P -Y;
    M2 = H -R -P +Y;
    M3 = H -R +P -Y;
    M4 = H +R +P +Y;
    u = [M1 M2 M3 M4]';
end