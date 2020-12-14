function R = rpy2R(roll, pitch, yaw)
    cr = cos(yaw); cy = cos(roll); cp = cos(pitch);
    sr = sin(yaw); sy = sin(roll); sp = sin(pitch);
    
    R = [cr*cp cr*sp*sy-sr*cy cr*sp*cy+sr*sy;
        sr*cp sr*sp*sy+cr*cy sr*sp*cy-cr*sy;
        -sp cp*sy cp*cy];
    
    
    
end