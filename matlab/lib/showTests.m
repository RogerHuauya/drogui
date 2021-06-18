function showTests(datos)
    mat = strings(size(datos));
    for i = 1:length(datos)
        mat(i, 1) = i;
        mat(i, 2) = datos(i).rpy;
        mat(i, 3) = datos(i).wrpy;
        mat(i, 4) = datos(i).xy;
        mat(i, 5) = datos(i).z;
        mat(i, 6) = datos(i).description;
        mat(i, 7) = datos(i).time;
    end
    t = array2table(mat, 'VariableNames', {'index', 'rpy' 'wrpy' 'xy' 'z' 'desc' 'time'});
    disp(t)
end