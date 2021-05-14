name_rpy = dir('*rpy.txt').name;
datos_rpy = appendTest(datos_rpy, name_rpy, ' 400 30 -10  ', '25 80 1500','0 0 0 ' , '0 0 0');
T1 = datos_rpy(end).datos;
name_xyz = dir('*xyz.txt').name;
datos_xyz = appendTest(datos_xyz, name_xyz, ' 400 30 -10  ', '30 80 1500', '0.25 0.05 0.3 ', '0.1 1 2 ');
T2 = datos_xyz(end).datos;
delete(name_rpy);
delete(name_xyz);