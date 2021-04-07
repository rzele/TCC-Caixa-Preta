classdef Gyroscope < CommonsLine
    properties
        filter_type % pode ser 'media' ou 'mediana'
        
        filter
        g_bias
        esc_giro
    end

    methods
        function obj = Gyroscope(filter_type, w_size, f_size, g_bias, esc_giro)
            p_title = sprintf('Giro em graus/s(%s=%d)', filter_type, f_size);
            obj = obj@CommonsLine(...
                p_title, ...                   % p_title
                'Amostra', ...                 % p_xlabel
                'graus/s', ...                 % p_ylabel
                {'gX', 'gY', 'gZ'}, ...        % s_legend
                {'r', 'g', 'b'}, ...           % sources_color
                w_size);

            obj.filter_type = filter_type;
            obj.w_size = w_size;
            obj.data = zeros(w_size, 3);
            obj.g_bias = g_bias;
            obj.esc_giro = esc_giro;

            if strcmp(filter_type, 'media')
                obj.filter = MovingAverage(f_size, 3);
            elseif strcmp(filter_type, 'mediana')
                obj.filter = MovingMedian(f_size, 3);
            else
                error('Invalid filter_type must be media or mediana');
            end
        end

        function calculate(obj, mpu_new_data, baselines_new_data, n_sample)
            %% Verifica se já calculou essa amostra
            if obj.has_calculated_this_sample(n_sample)
                return
            end

            %% Calcula o valor p/ a próxima amostra
            G = mpu_new_data(4:6);
            last_data = obj.last();
            new_data = calculate_gyro(G, obj.g_bias, obj.esc_giro);
            filtered = obj.filter.calculate(new_data);
            obj.data = [obj.data(2:obj.w_size, :); filtered];
        end
    end
end