name = dir('*rpy.txt').name;
datos = appendTest(datos,name, '200 0 -1000', '30 25 1500','0 0 0 ' , '0 0 0');
T = datos(end).datos;
delete(name);
delete(strrep(name, "rpy", "xyz"));