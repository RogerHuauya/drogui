rpyfile = dir('*rpy.txt').name;
xyzfile = dir('*xyz.txt').name;

rpy =   '400 30 -10';
wrpy =  '30 80 1500';
xy  =   '0.2 0.05 0.2'; 
z =     '0.1 0.1 0.1';

rpydat = appendTest(rpydat, rpyfile, rpy, wrpy, xy, z);
xyzdat = appendTest(xyzdat, xyzfile, rpy, wrpy, xy, z);

Trpy = rpydat(end).datos;
Txyz = xyzdat(end).datos;

delete(rpyfile);
delete(xyzfile);