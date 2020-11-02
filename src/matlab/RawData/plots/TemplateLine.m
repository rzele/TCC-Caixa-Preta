classdef TemplateLine < Template
    properties
        w_size
    end

    methods
        function obj = TemplateLine()
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