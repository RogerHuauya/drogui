rpyfile = dir('*rpy.txt').name;
xyzfile = dir('*xyz.txt').name;

rpy =   '400 30 -10';
wrpy =  '30 80 1500';
xy  =   '0.25 0.05 0.3'; 
z =     '50 20 2000';

datos_rpy = appendTest(datos_rpy, rpyfile, rpy, wrpy, xy, z);
datos_xyz = appendTest(datos_rpy, xyzfile, rpy, wrpy, xy, z);

Trpy = datos_rpy(end).datos;
Txyz = datos_xyz(end).datos;

delete(rpyfile);
delete(xyzfile);
