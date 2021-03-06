classdef Aceleration < CommonsLine
    % Esta classe realizar cálculos de aceleração
    % e plotagem de um gráfico em linha na tela

    properties
        moving_avg
        a_bias
        esc_ac
    end

    methods
        function obj = Aceleration(w_size, f_size, a_bias, esc_ac)
            obj = obj@CommonsLine(...
                'Aceleração em "g"', ...       % p_title
                'Amostra', ...                 % p_xlabel
                'g', ...                       % p_ylabel
                {'aX', 'aY', 'aZ'}, ...        % s_legend
                {'r', 'g', 'b'});              % sources_color

            obj.w_size = w_size;
            obj.moving_avg = MovingAverage(f_size, 3);
            obj.data = zeros(w_size, 3);
            obj.a_bias = a_bias;
            obj.esc_ac = esc_ac;
        end

        function calculate(obj, mpu_new_data, n_sample)
            %% Verifica se já calculou essa amostra
            if obj.has_calculated_this_sample(n_sample)
                return
            end

            %% Calcula o valor p/ a próxima amostra
            A = mpu_new_data(1:3);
            last_data = obj.last();
            new_data = obj.calculate_aceleration(A, obj.a_bias, obj.esc_ac);
            filtered = obj.moving_avg.update(new_data);
            obj.data = [obj.data(2:obj.w_size, :); filtered];
        end
    end
end