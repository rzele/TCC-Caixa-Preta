classdef GyroRelativeTilt < CommonsLine
    % Calcula Yaw, Pitch e Roll realtivos(em rela��o a ultima Rota��o do corpo) p/ a nova amostra usando giro
    % Usando o giro, fazemos a integral discreta (�rea do trap�zio aculmulado)
    % P/ o novo dado isso significa, ultimo valor + novo trap�zio (entre ultimo dado e o novo)
    % � considerado nesse calculo que, as amostragens est�o espa�adas de 1
    % periodo da amostragem, ent o trap�zio � igual a 1/freq * ((n-1 + n)/2)

    properties
        freq_sample

        % Chart dependences obj
        gyro_chart
    end

    methods
        function obj = GyroRelativeTilt(w_size, freq_sample, gyro_chart)
            obj = obj@CommonsLine(...
                'Inclina��o Relativa', ...          % p_title
                'Amostra', ...                      % p_xlabel
                'graus', ...                        % p_ylabel
                {'Roll', 'Pitch', 'Yaw'}, ...       % s_legend
                {'r', 'g', 'b'}, ...                % sources_color
                w_size);

            obj.w_size = w_size;
            obj.data = zeros(w_size, 3);
            obj.freq_sample = freq_sample;

            % Chart dependences
            obj.gyro_chart = gyro_chart;
        end

        function calculate(obj, mpu_new_data, baselines_new_data, n_sample)
            %% Verifica se j� calculou essa amostra
            if obj.has_calculated_this_sample(n_sample)
                return
            end

            %% Obtem o valor de outros charts ao qual este � dependente
            obj.gyro_chart.calculate(mpu_new_data, baselines_new_data, n_sample);
            G = obj.gyro_chart.last();
            old_G = obj.gyro_chart.penult();
            
            %% Calcula o valor p/ a pr�xima amostra
            new_data = calculate_gyro_relative_tilt(obj.last(), G, old_G, obj.freq_sample);
            obj.data = [obj.data(2:obj.w_size, :); new_data];
        end
    end
end