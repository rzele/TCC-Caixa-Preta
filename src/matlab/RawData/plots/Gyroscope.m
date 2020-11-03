classdef Gyroscope < TemplateLine
    properties
        moving_avg
    end

    methods
        function obj = Gyroscope()
            obj.name = 'Gyroscope';
        end

        function obj = initialize(obj, fig, w_size, f_size)
            obj.w_size = w_size;
            obj.my_plot = fig.setItemType(obj, obj.name, 'plotline');
            obj.my_plot.configPlot('Giro em graus/seg', 'Amostra', 'graus/seg', {'gX', 'gY', 'gZ'}, {'r', 'g', 'b'});
            obj.moving_avg = MovingAverage(f_size, 3);
            obj.data = zeros(w_size, 3);
        end

        function calculate(obj, G, g_bias, esc_giro)
            last_data = obj.last();
            new_data = calculate_gyro(G, g_bias, esc_giro);
            filtered = obj.moving_avg.update(new_data);
            obj.data = [obj.data(2:obj.w_size, :); filtered];
        end
    end
end