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
            plot(obj.datos.ROLL_SCURVE);
            hold on
            plot(obj.datos.ROLL_VAL);
        end
        function plotPitch(obj)
            plot(obj.datos.PITCH_SCURVE);
            hold on
            plot(obj.datos.PITCH_VAL);
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