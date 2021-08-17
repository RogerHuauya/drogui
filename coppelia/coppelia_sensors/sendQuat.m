while(1)
    str = readline(device);
    splt_str = split(str,',');
    quaternion = str2double(splt_str);
    
    if(length(quaternion) == 4)
        quaternion = [quaternion(2:4); quaternion(1)];
        sendCoppeliaQuat(sim, clientID, quaternion)
    end
    pause(0.1)
    flush(device)
end