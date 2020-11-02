classdef Magnetometer < TemplateLine
    properties
        moving_avg
    end

    methods
        function obj = Magnetometer()
            obj.name = 'Magnetometer';
        end

        function obj = initialize(obj, fig, w_size, f_size)
            obj.w_size = w_size;
            obj.my_plot = fig.setItemType(obj.name, 'plotline');
            obj.my_plot.configPlot('Magnetrometro em mili Gaus', 'Amostra', 'mG', {'hx', 'hy', 'hz'}, {'r', 'g', 'b'});
            obj.moving_avg = MovingAverage(f_size, 3);
            obj.data = zeros(w_size, 3);
        end

        function calculate(obj, H, h_offsets, h_scaleso)
            last_data = obj.last();
            new_data = calculate_mag(H, h_offsets, h_scaleso);
            filtered = obj.moving_avg.update(new_data);
            obj.data = [obj.data(2:obj.w_size, :); filtered];
        end
    end
end