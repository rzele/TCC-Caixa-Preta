classdef GyroAbsoluteTilt < CommonsLine
    % Calcula a matriz de Rotação (Z,Y,X) que foi responsável por mover o corpo da Posição da amostra anterior para a atual
    % Ref do calculo: https://www.youtube.com/watch?v=wg9bI8-Qx2Q

    properties
        % Chart dependences obj
        relative_tilt_chart
    end

    methods
        function obj = GyroAbsoluteTilt(w_size, relative_tilt_chart)
            obj = obj@CommonsLine(...
                'Inclinação Absoluta (gyro)', ...      % p_title
                'Amostra', ...                                % p_xlabel
                'graus', ...                                  % p_ylabel
                {'Roll', 'Pitch', 'Yaw'}, ...                 % s_legend
                {'r', 'g', 'b'});                             % sources_color

            obj.w_size = w_size;
            obj.data = zeros(w_size, 3);
            
            % Chart dependences
            obj.relative_tilt_chart = relative_tilt_chart;
        end

        function calculate(obj, mpu_new_data, n_sample)
            %% Verifica se já calculou essa amostra
            if obj.has_calculated_this_sample(n_sample)
                return
            end

            %% Obtem o valor de outros charts ao qual este é dependente
            obj.relative_tilt_chart.calculate(mpu_new_data, n_sample);
            relative_tilt = obj.relative_tilt_chart.last();
            
            %% Calcula o valor p/ a próxima amostra
            new_data = calculate_gyro_absolute_tilt(relative_tilt);
            obj.data = [obj.data(2:obj.w_size, :); new_data];
        end
    end
end