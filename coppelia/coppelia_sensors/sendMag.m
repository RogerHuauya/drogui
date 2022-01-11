while(1)
    str = readline(device);
    splt_str = split(str,',');
    mag = str2double(splt_str)
    
    if(length(mag) == 3)
        sendCoppeliaMag(sim, clientID, mag)
    end
    pause(0.1)
    flush(device)
end