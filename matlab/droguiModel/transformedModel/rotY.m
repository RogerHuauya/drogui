function [R] = rotY(a)
    R = [cos(a) 0 sin(a);
        0 1 0
        - sin(a) 0 cos(a)];
end