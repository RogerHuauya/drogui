function retarr = appendTest(datos, nombre,rpy, wrpy, xy, z)
    retarr = [datos; importTest(nombre, rpy, wrpy, xy, z)];
end