classdef CompassCompensated < TemplateCompass
    % Usa o valor calculado do compass com compensação já calculado acima
    % Ref do calculo: https://www.mikrocontroller.net/attachment/292888/AN4248.pdf

    properties
    end

    methods
        function obj = CompassCompensated()
            obj = obj@TemplateCompass('Magnetic Heading COM compensação de tilt');      % p_title
            obj.data = 0;
        end

        function calculate(obj, yaw)
            obj.data = yaw;
        end
    end
end