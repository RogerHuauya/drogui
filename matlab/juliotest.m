fs = 50;
disp('0. Fir1');
disp('1. Bessel');
disp('2. Butter');
disp('3. Cheby1');
disp('4. Cheby2');
prototype = input('Ingrese el prototipo: ');

for fc=10:10:100

    for grade=2:2:8
        wn = ((fc+2)/(fs/2));
        fprintf('Frecuencia de corte: %d y grado del filtro: %d \n',fc,grade)
        axf = filterSignal(ax,prototype,grade,wn);
        [freqs,AX_F] = fourier(ax,fs);
        [freqsf,AXF_F] = fourier(axf,fs);
        figure(1)
        title('Señal sin filtrar');
        sp1_1 = subplot(2,1,1);plot(ax,'g');
        sp1_2 = subplot(2,1,2);plot(freqs,AX_F,'r');
        title(sp1_1,'Señal sin filtrar');
        title(sp1_2,'Espectro en fourier');
        figure(2)
        sp2_1 = subplot(2,1,1);plot(axf,'g');
        sp2_2 = subplot(2,1,2);plot(freqsf,AXF_F,'r');
        title(sp2_1,'Señal filtrada');
        title(sp2_2,'Espectro en fourier');
        figure(3)
        plot(ax-axf,'b');
        hold on;
        key = input('Desea continuar: ')
        if key == 1
            continue;
        elseif key == 0
            break;
        end
        close all;
    end
    if key == 0
            break;
    end
end