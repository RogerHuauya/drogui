function T = importTable(name)
    T = readtable(name);
    T = fillmissing(T, 'previous');
end