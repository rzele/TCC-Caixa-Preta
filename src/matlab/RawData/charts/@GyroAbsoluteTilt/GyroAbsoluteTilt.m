classdef GyroAbsoluteTilt < TemplateLine
    % Calcula a matriz de Rotação (Z,Y,X) que foi responsável por mover o corpo da Posição da amostra anterior para a atual
    % Ref do calculo: https://www.youtube.com/watch?v=wg9bI8-Qx2Q

    properties
    end

    methods
        function obj = GyroAbsoluteTilt(w_size)
            obj = obj@TemplateLine(...
                'Giro em graus(absoluto)', ...      % p_title
                'Amostra', ...                      % p_xlabel
                'graus', ...                        % p_ylabel
                {'Roll', 'Pitch', 'Yaw'}, ...       % s_legend
                {'r', 'g', 'b'});                   % sources_color

            obj.w_size = w_size;
            obj.data = zeros(w_size, 3);
        end

        function calculate(obj, relative_tilt)
            new_data = obj.calculate_gyro_absolute_tilt(relative_tilt);
            obj.data = [obj.data(2:obj.w_size, :); new_data];
        end
    end
end