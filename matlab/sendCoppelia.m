function sendCoppelia(sim,  clientID, pos, ori)
    [in, fl, nos, str] = sim.simxCallScriptFunction(clientID,'Quadricopter', ...
                    sim.sim_scripttype_childscript,'setDroguiPosition', ...
                    [],[pos,ori],[],[],sim.simx_opmode_blocking);
    disp([in, fl, nos, str])
end

