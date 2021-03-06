classdef Position < CommonsLine
    % Calcula Posi��o integrando a Velocidade
    % Usando a Velocidade, fazemos a integral discreta (�rea do trap�zio aculmulado)
    % P/ o novo dado isso significa, ultimo valor + novo trap�zio (entre ultimo dado e o novo)
    % � considerado nesse calculo que, as amostragens est�o espa�adas de 1
    % periodo da amostragem, ent o trap�zio � igual a 1/freq * ((n-1 + n)/2)

    properties
        freq_sample
        
        % Chart dependences obj
        vel_chart
    end

    methods
        function obj = Position(w_size, freq_sample, vel_chart)
            obj = obj@CommonsLine(...
                'Espa�o em metros', ...     % p_title
                'Amostra', ...              % p_xlabel
                'metros', ...               % p_ylabel
                {'X', 'Y', 'Z'}, ...        % s_legend
                {'r', 'g', 'b'});           % sources_color

            obj.w_size = w_size;
            obj.data = zeros(w_size, 3);
            obj.freq_sample = freq_sample;

            % Chart dependences
            obj.vel_chart = vel_chart;
        end

        function calculate(obj, mpu_new_data, n_sample)
             %% Verifica se j� calculou essa amostra
             if obj.has_calculated_this_sample(n_sample)
                return
            end

            %% Obtem o valor de outros charts ao qual este � dependente
            obj.vel_chart.calculate(mpu_new_data, n_sample);
            velocity = obj.vel_chart.last();
            old_velocity = obj.vel_chart.penult();
            
            %% Calcula o valor p/ a pr�xima amostra
            new_data = obj.calculate_position(obj.last(), velocity, old_velocity, obj.freq_sample);
            obj.data = [obj.data(2:obj.w_size, :); new_data];
        end
    end
end