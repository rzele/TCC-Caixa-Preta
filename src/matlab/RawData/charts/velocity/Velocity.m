classdef Velocity < CommonsLine
    % Calcula Velocidade integrando a Acelera��o
    % Usando a Acelera��o, fazemos a integral discreta (�rea do trap�zio aculmulado)
    % P/ o novo dado isso significa, ultimo valor + novo trap�zio (entre ultimo dado e o novo)
    % � considerado nesse calculo que, as amostragens est�o espa�adas de 1
    % periodo da amostragem, ent o trap�zio � igual a 1/freq * ((n-1 + n)/2)

    properties
        freq_sample
        const_g

        % Chart dependences obj
        acel_without_g_chart
    end

    methods
        function obj = Velocity(w_size, freq_sample, const_g, acel_without_g_chart)
            obj = obj@CommonsLine(...
                'Velocidade em m/s', ...       % p_title
                'Amostra', ...                 % p_xlabel
                'm/s', ...                     % p_ylabel
                {'vX', 'vY', 'vZ'}, ...        % s_legend
                {'r', 'g', 'b'}, ...           % sources_color
                w_size);

            obj.w_size = w_size;
            obj.data = zeros(w_size, 3);
            obj.freq_sample = freq_sample;
            obj.const_g = const_g;

            % Chart dependences
            obj.acel_without_g_chart = acel_without_g_chart;
        end

        function calculate(obj, mpu_new_data, baselines_new_data, n_sample)
            %% Verifica se j� calculou essa amostra
            if obj.has_calculated_this_sample(n_sample)
                return
            end
            
            %% Obtem o valor de outros charts ao qual este � dependente
            obj.acel_without_g_chart.calculate(mpu_new_data, baselines_new_data, n_sample);
            A_without_gravity = obj.acel_without_g_chart.last();
            old_A_without_gravity = obj.acel_without_g_chart.penult();

            % Obtem acelera��o em m/s^2 at� aqui a acelera��o era dada em escalas de g's
            A_ms = obj.const_g * A_without_gravity;
            old_A_ms = obj.const_g * old_A_without_gravity;
            
            %% Calcula o valor p/ a pr�xima amostra
            new_data = calculate_velocity(obj.last(), A_ms, old_A_ms, obj.freq_sample);
            obj.data = [obj.data(2:obj.w_size, :); new_data];
        end
    end
end