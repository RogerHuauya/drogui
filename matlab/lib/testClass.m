classdef testClass
    properties
        datos
        rpy
        wrpy
        xy
        z
        description = ""
        time = ""
    end
    methods
        function plotRoll(obj)
            figure('Name', "roll " + obj.rpy,'NumberTitle','off');
            
            plot(obj.datos.TIME,obj.datos.ROLL_SCURVE);
            hold on
            plot(obj.datos.TIME,obj.datos.ROLL_VAL);
            hold off;
        end
        function plotPitch(obj)
            figure('Name',"pitch " + obj.rpy,'NumberTitle','off');
            plot(obj.datos.TIME,obj.datos.PITCH_SCURVE);
            hold on
            plot(obj.datos.TIME,obj.datos.PITCH_VAL);
          
            hold off
        end
        
        
        function plotGyroX(obj)
            figure('Name',"Gyro x " + obj.wrpy,'NumberTitle','off');
            plot(obj.datos.TIME,obj.datos.GYRO_X_REF);
            hold on
            plot(obj.datos.TIME,obj.datos.GYRO_X);
          
            hold off;
        end
        function plotGyroY(obj)
            figure('Name', "Gyro y " + obj.wrpy,'NumberTitle','off');
            plot(obj.datos.TIME,obj.datos.GYRO_Y_REF);
            hold on
            plot(obj.datos.TIME,obj.datos.GYRO_Y);
            
            hold off;
        end
        
        
        function plotX(obj)
            plot(obj.datos.TIME,obj.datos.X_SCURVE);
            hold on
            plot(obj.datos.TIME,obj.datos.X_VAL);
        end
        function plotY(obj)
            plot(obj.datos.TIME,obj.datos.Y_SCURVE);
            hold on
            plot(obj.datos.TIME,obj.datos.Y_VAL);
        end
    end
end