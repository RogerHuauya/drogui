function plotHold(mat) 
    colors = ['b', 'r', 'g', 'c', 'm', 'y'];
    cant = size(mat,2);
    for i = 1:cant
        plot(mat(:,i), colors(i))
        hold on;
    end
    hold off
end