name = dir('*rpy.txt').name;
datos = appendTest(datos,name, ' 0 0 0 ', '3 100 1500 Yaw 1 50 500','0 0 0 ' , '0 0 0');
T = datos(end).datos;
delete(name);
delete(strrep(name, "rpy", "xyz"));