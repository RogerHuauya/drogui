function R = rpy2R(roll, pitch, yaw)
    cy = cos(yaw); cr = cos(roll); cp = cos(pitch);
    sy = sin(yaw); sr = sin(roll); sp = sin(pitch);
    
    R = [cy*cp cy*sp*sr-sy*cr cy*sp*cr+sy*sr;
        sy*cp sy*sp*sr+cy*cr sy*sp*cr-cy*sr;
        -sp cp*sr cp*cr];
end