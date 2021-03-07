classdef Gyroscope < CommonsLine
    properties
        moving_avg
        g_bias
        esc_giro
    end

    methods
        function obj = Gyroscope(w_size, f_size, g_bias, esc_giro)
            obj = obj@CommonsLine(...
                'Giro em graus/s', ...         % p_title
                'Amostra', ...                 % p_xlabel
                'graus/s', ...                 % p_ylabel
                {'gX', 'gY', 'gZ'}, ...        % s_legend
                {'r', 'g', 'b'}, ...           % sources_color
                w_size);

            obj.w_size = w_size;
            obj.moving_avg = MovingAverage(f_size, 3);
            obj.data = zeros(w_size, 3);
            obj.g_bias = g_bias;
            obj.esc_giro = esc_giro;
        end

        function calculate(obj, mpu_new_data, n_sample)
            %% Verifica se já calculou essa amostra
            if obj.has_calculated_this_sample(n_sample)
                return
            end

            %% Calcula o valor p/ a próxima amostra
            G = mpu_new_data(4:6);
            last_data = obj.last();
            new_data = calculate_gyro(G, obj.g_bias, obj.esc_giro);
            filtered = obj.moving_avg.update(new_data);
            obj.data = [obj.data(2:obj.w_size, :); filtered];
        end
    end
end