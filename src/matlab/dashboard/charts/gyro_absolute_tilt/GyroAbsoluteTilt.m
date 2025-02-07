classdef GyroAbsoluteTilt < CommonsLine
    % Calcula a matriz de Rota��o (Z,Y,X) que foi respons�vel por mover o corpo da Posi��o da amostra anterior para a atual
    % Ref do calculo: https://www.youtube.com/watch?v=wg9bI8-Qx2Q

    properties
    end

    methods
        function obj = GyroAbsoluteTilt(w_size, relative_tilt_chart)
            obj = obj@CommonsLine(...
                'Inclina��o Absoluta (gyro)', ...             % p_title
                'Amostra', ...                                % p_xlabel
                'graus', ...                                  % p_ylabel
                {'Roll', 'Pitch', 'Yaw'}, ...                 % s_legend
                {'r', 'g', 'b'}, ...                          % sources_color
                w_size);

            obj.w_size = w_size;
            obj.data = zeros(w_size, 3);
            
            % Chart dependences
            obj.dependencies.relative_tilt = relative_tilt_chart;
        end

        function calculate(obj, mpu_new_data, baselines_new_data, n_sample)
            %% Verifica se j� calculou essa amostra
            if obj.has_calculated_this_sample(n_sample)
                return
            end

            %% Obtem o valor de outros charts ao qual este � dependente
            obj.dependencies.relative_tilt.calculate(mpu_new_data, baselines_new_data, n_sample);
            rel_tilt = obj.dependencies.relative_tilt.last();
            old_rel_tilt = obj.dependencies.relative_tilt.penult();
            
            %% Calcula o valor p/ a pr�xima amostra
            t = tic();
           
            new_data = calculate_gyro_absolute_tilt(rel_tilt, old_rel_tilt, obj.last());
           
            obj.time = obj.time + toc(t);
            obj.data = [obj.data(2:obj.w_size, :); new_data];
        end
    end
end