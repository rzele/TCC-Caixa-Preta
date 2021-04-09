classdef CompTilt < CommonsLine
    % Calcula Rotação usando filtro complementar
    % Ref do calculo: https://www.youtube.com/watch?v=whSw42XddsU

    properties
        mu
    end

    methods
        function obj = CompTilt(w_size, mu, relative_tilt_chart, acel_mag_tilt_chart)
            obj = obj@CommonsLine(...
                'Filtro Complementar', ...          % p_title
                'Amostra', ...                      % p_xlabel
                'graus', ...                        % p_ylabel
                {'Roll', 'Pitch', 'Yaw'}, ...       % s_legend
                {'r', 'g', 'b'}, ...                % sources_color
                w_size);

            obj.w_size = w_size;
            obj.data = zeros(w_size, 3);
            obj.mu = mu;

            % Chart dependences
            obj.dependencies.relative_tilt = relative_tilt_chart;
            obj.dependencies.acel_mag_tilt = acel_mag_tilt_chart;
        end
        
        function calculate(obj, mpu_new_data, baselines_new_data, n_sample)
            %% Verifica se já calculou essa amostra
            if obj.has_calculated_this_sample(n_sample)
                return
            end
            
            %% Obtem o valor de outros charts ao qual este é dependente
            obj.dependencies.relative_tilt.calculate(mpu_new_data, baselines_new_data, n_sample);
            gyro_tilt = obj.dependencies.relative_tilt.last();
            old_gyro_tilt = obj.dependencies.relative_tilt.penult();

            obj.dependencies.acel_mag_tilt.calculate(mpu_new_data, baselines_new_data, n_sample);
            tilt = obj.dependencies.acel_mag_tilt.last();

            %% Calcula o valor p/ a próxima amostra
            t = tic();
            
            new_data = calculate_comp_filter(obj.last(), gyro_tilt, old_gyro_tilt, tilt, obj.mu);
            
            obj.time = obj.time + toc(t);
            obj.data = [obj.data(2:obj.w_size, :); new_data];
        end
    end
end