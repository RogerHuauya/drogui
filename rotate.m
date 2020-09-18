close all, clear all
[n,wh] = vrimport(which('Drone complete matlab.STL'));
open(wh);
view(wh)
for i = 1:10
    n.rotation = [0 i*10 0 0.1]
    pause(1);
end

display(hola)

