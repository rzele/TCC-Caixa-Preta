classdef GyroAbsoluteTilt < TemplateLine
    % Calcula a matriz de Rotação (Z,Y,X) que foi responsável por mover o corpo da Posição da amostra anterior para a atual
    % Ref do calculo: https://www.youtube.com/watch?v=wg9bI8-Qx2Q

    properties
    end

    methods
        function obj = GyroAbsoluteTilt()
            obj.name = 'GyroAbsoluteTilt';
        end

        function obj = initialize(obj, fig, w_size)
            obj.w_size = w_size;
            obj.my_plot = fig.setItemType(obj, obj.name, 'plotline');
            obj.my_plot.configPlot('Giro em graus(absoluto)', 'Amostra', 'graus', {'Roll', 'Pitch', 'Yaw'}, {'r', 'g', 'b'});
            obj.data = zeros(w_size, 3);
        end

        function calculate(obj, relative_tilt)
            new_data = calculate_gyro_absolute_tilt(relative_tilt);
            obj.data = [obj.data(2:obj.w_size, :); new_data];
        end
    end
end