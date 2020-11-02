classdef TemplateLine < handle
    properties
        name
        data
        w_size
        my_plot
    end

    methods
        function obj = TemplateLine()
        end

        function obj = initialize(obj, fig, w_size)
        end

        function ret = calculate(obj, gyro_tilt, old_gyro_tilt, tilt, mu)
        end

        function update(obj)
            dataTransposed = obj.data';
            obj.my_plot.update(dataTransposed);
        end

        function ret = last(obj)
            ret = obj.data(obj.w_size, :);
        end

        function ret = penult(obj)
            ret = obj.data(obj.w_size-1, :);
        end
    end
end