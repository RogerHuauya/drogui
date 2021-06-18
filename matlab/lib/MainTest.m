
if exist('Drpy','var') ~= 1
    Drpy = [];
end
if exist('Dxyz','var') ~= 1
    Dxyz = [];
end

rpyfiles = dir('*rpy.txt');
xyzfiles = dir('*xyz.txt');

for i = 1: length(rpyfiles)
    rpyfile = rpyfiles(i).name;
    xyzfile = xyzfiles(i).name;
    rpy =   '500/300 20 -1000';
    wrpy =  '5/8/10 10/16/30 500';
    xy  =   '0.2 0.08 1.5'; 
    z =     '10 2 750 Comp 28';

    Drpy = appendTest(Drpy, rpyfile, rpy, wrpy, xy, z);
    Dxyz = appendTest(Dxyz, xyzfile, rpy, wrpy, xy, z);


    delete(rpyfile);
    delete(xyzfile);
end



Trpy = Drpy(end).datos;
Txyz = Dxyz(end).datos;

