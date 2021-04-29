function plotSync(mat) 
    colors = ['b', 'r', 'g', 'c', 'm', 'y'];
    cant = size(mat,2);
    tiledlayout(cant, 1)
    axes = zeros(cant,1);
    for i = 1:cant
        axes(i) = nexttile;
        plot(mat(:,i), colors(i))
    end
    linkaxes(axes,'x')
end