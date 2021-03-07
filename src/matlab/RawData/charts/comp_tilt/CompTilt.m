classdef CompTilt < CommonsLine
    % Calcula Rotação usando filtro complementar
    % Ref do calculo: https://www.youtube.com/watch?v=whSw42XddsU

    properties
        mu

        % Chart dependences obj
        relative_tilt_chart
        acel_mag_tilt_chart
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
            obj.relative_tilt_chart = relative_tilt_chart;
            obj.acel_mag_tilt_chart = acel_mag_tilt_chart;
        end
        
        function calculate(obj, mpu_new_data, n_sample)
            %% Verifica se já calculou essa amostra
            if obj.has_calculated_this_sample(n_sample)
                return
            end
            
            %% Obtem o valor de outros charts ao qual este é dependente
            obj.relative_tilt_chart.calculate(mpu_new_data, n_sample);
            gyro_tilt = obj.relative_tilt_chart.last();
            old_gyro_tilt = obj.relative_tilt_chart.penult();

            obj.acel_mag_tilt_chart.calculate(mpu_new_data, n_sample);
            tilt = obj.acel_mag_tilt_chart.last();

            %% Calcula o valor p/ a próxima amostra
            new_data = calculate_comp_filter(obj.last(), gyro_tilt, old_gyro_tilt, tilt, obj.mu);
            obj.data = [obj.data(2:obj.w_size, :); new_data];
        end
    end
end