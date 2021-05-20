rpyfile = dir('*rpy.txt').name;
xyzfile = dir('*xyz.txt').name;

rpy =   '0 0 0';
wrpy =  '10 20 500';
xy  =   '0 0 0'; 
z =     '0 0 0';

Drpy = appendTest(Drpy, rpyfile, rpy, wrpy, xy, z);
Dxyz = appendTest(Dxyz, xyzfile, rpy, wrpy, xy, z);

Trpy = Drpy(end).datos;
Txyz = Dxyz(end).datos;

delete(rpyfile);
delete(xyzfile);
