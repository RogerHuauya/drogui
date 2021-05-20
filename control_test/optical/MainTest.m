rpyfile = dir('*rpy.txt').name;
xyzfile = dir('*xyz.txt').name;

rpy =   '300 15 -5';
wrpy =  '15 40 750';
xy  =   '0.2 0.05 0.3'; 
z =     '0 0 0';

datos_rpy = appendTest(datos_rpy, rpyfile, rpy, wrpy, xy, z);
datos_xyz = appendTest(datos_xyz, xyzfile, rpy, wrpy, xy, z);

Trpy = datos_rpy(end).datos;
Txyz = datos_xyz(end).datos;

delete(rpyfile);
delete(xyzfile);
