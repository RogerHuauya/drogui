if exist('Drpy','var') ~= 1
    Drpy = [];
end
if exist('Dxyz','var') ~= 1
    Dxyz = [];
end
rpyfile = dir('*rpy.txt').name;
xyzfile = dir('*xyz.txt').name;

rpy =   '500/300 20 -1000';
wrpy =  '5/8 10/16 500';
xy  =   '0.2 0.05 0.3'; 
z =     '0 0 0';

Drpy = appendTest(Drpy, rpyfile, rpy, wrpy, xy, z);
Dxyz = appendTest(Dxyz, xyzfile, rpy, wrpy, xy, z);

Trpy = Drpy(end).datos;
Txyz = Dxyz(end).datos;

delete(rpyfile);
delete(xyzfile);
