function showTests(datos)
    mat = strings(size(datos));
    for i = 1:length(datos)
        mat(i, 1) = datos(i).rpy;
        mat(i, 2) = datos(i).wrpy;
        mat(i, 3) = datos(i).xy;
        mat(i, 4) = datos(i).z;
        mat(i, 5) = datos(i).description;
    end
    t = array2table(mat, 'VariableNames', {'rpy' 'wrpy' 'xy' 'z' 'desc'});
    disp(t)
end