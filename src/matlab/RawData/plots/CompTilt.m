classdef CompTilt < TemplateLine
    % Calcula Rotação usando filtro complementar
    % Ref do calculo: https://www.youtube.com/watch?v=whSw42XddsU

    properties
    end

    methods
        function obj = CompTilt()
            obj.name = 'CompTilt';
        end

        function obj = initialize(obj, fig, w_size)
            obj.w_size = w_size;
            obj.my_plot = fig.setItemType(obj.name, 'plotline');
            obj.my_plot.configPlot('Filtro complementar', 'Amostra', 'graus', {'Roll', 'Pitch', 'Yaw'}, {'r', 'g', 'b'});
            obj.data = zeros(w_size, 3);
        end

        function calculate(obj, gyro_tilt, old_gyro_tilt, tilt, mu)
            new_data = calculate_comp_filter(obj.last(), gyro_tilt, old_gyro_tilt, tilt, mu);
            obj.data = [obj.data(2:obj.w_size, :); new_data];
        end
    end
end