classdef Aceleration < TemplateLine
    properties
        moving_avg
    end

    methods
        function obj = Aceleration()
            obj.name = 'Aceleration';
        end

        function obj = initialize(obj, fig, w_size, f_size)
            obj.w_size = w_size;
            obj.my_plot = fig.setItemType(obj, obj.name, 'plotline');
            obj.my_plot.configPlot('Aceleração em "g"', 'Amostra', 'g', {'vX', 'vY', 'vZ'}, {'r', 'g', 'b'});
            obj.moving_avg = MovingAverage(f_size, 3);
            obj.data = zeros(w_size, 3);
        end

        function calculate(obj, A, a_bias, esc_ac)
            last_data = obj.last();
            new_data = calculate_aceleration(A, a_bias, esc_ac);
            filtered = obj.moving_avg.update(new_data);
            obj.data = [obj.data(2:obj.w_size, :); filtered];
        end
    end
end