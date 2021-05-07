classdef testClass
    properties
        datos
        rpy
        wrpy
        xy
        z
        description = ""
    end
    methods
        function plotRoll(obj)
            figure;
            
            plot(obj.datos.ROLL_SCURVE);
            hold on
            plot(obj.datos.ROLL_VAL);
            title(strcat('roll ', obj.rpy))
            hold off;
        end
        function plotPitch(obj)
            figure;
            plot(obj.datos.PITCH_SCURVE);
            hold on
            plot(obj.datos.PITCH_VAL);
            title(strcat('pitch ',obj.rpy))
            hold off
        end
        
        
        function plotGyroX(obj)
            figure;
            plot(obj.datos.GYRO_X_REF);
            hold on
            plot(obj.datos.GYRO_X);
            title(strcat('Gyro x ',obj.wrpy))
            hold off;
        end
        function plotGyroY(obj)
            figure;
            plot(obj.datos.GYRO_Y_REF);
            hold on
            plot(obj.datos.GYRO_Y);
            title(strcat('Gyro y ',obj.wrpy))
            hold off;
        end
        
        
        function plotX(obj)
            plot(obj.datos.X_SCURVE);
            hold on
            plot(obj.datos.X_VAL);
        end
        function plotY(obj)
            plot(obj.datos.Y_SCURVE);
            hold on
            plot(obj.datos.Y_VAL);
        end
    end
end