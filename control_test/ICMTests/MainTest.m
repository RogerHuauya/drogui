name = dir('*rpy.txt').name;
datos = appendTest(datos,name, ' 400 30 -10 ', '30 80 1500','0 0 0 ' , '0 0 0');
T = datos(end).datos;
delete(name);
delete(strrep(name, "rpy", "xyz"));