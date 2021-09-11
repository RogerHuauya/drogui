for windows=5:5:100 
    b = (1/windows)*ones(1,windows);
    a = 1;
    y = filter(b,a,az);
    hold on;
    plot(az,'b');
    plot(y,'r');
    fprintf('Ventana: %d \n',windows);
    key = input('Desea continuar: ');
    if key == 1
        continue;
    elseif key == 0
        break;
    end
    close all
end