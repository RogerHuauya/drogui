name = dir('*rpy.txt').name;
datos = appendTest(datos, name, ' 1000 500 500 ', '0 0 0','0 0 0 ' , '0 0 0');
T = datos(end).datos;
delete(name);
delete(strrep(name, "rpy", "xyz"));