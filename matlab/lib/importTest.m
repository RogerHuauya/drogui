function ret = importTest(nombre, rpy, wrpy, xy, z)
    ret = testClass;
    ret.rpy = rpy;
    ret.wrpy = wrpy;
    ret.xy = xy;
    ret.z = z;
    ret.datos = importTable(nombre);
end