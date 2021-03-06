classdef Gyroscope < TemplateLine
    properties
        moving_avg
    end

    methods
        function obj = Gyroscope(w_size, f_size)
            obj = obj@TemplateLine(...
                'Giro em graus/seg', ...       % p_title
                'Amostra', ...                 % p_xlabel
                'graus/seg', ...               % p_ylabel
                {'gX', 'gY', 'gZ'}, ...        % s_legend
                {'r', 'g', 'b'});              % sources_color

            obj.w_size = w_size;
            obj.moving_avg = MovingAverage(f_size, 3);
            obj.data = zeros(w_size, 3);
        end

        function calculate(obj, G, g_bias, esc_giro)
            last_data = obj.last();
            new_data = obj.calculate_gyro(G, g_bias, esc_giro);
            filtered = obj.moving_avg.update(new_data);
            obj.data = [obj.data(2:obj.w_size, :); filtered];
        end
    end
end