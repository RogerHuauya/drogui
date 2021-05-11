name = dir('*rpy.txt').name;
datos = appendTest(datos,name, ' 0 0 0 ', '1 20 0 Yaw 1 20 0','0 0 0 ' , '0 0 0');
T = datos(end).datos;
delete(name);
delete(strrep(name, "rpy", "xyz"));