classdef TemplateCar3D < Template
    properties
    end

    methods
        function obj = TemplateCar3D()
        end

        function calculate(obj, new_data)
            obj.data = new_data;
        end

        function update(obj)
            obj.my_plot.rotateWithEuler(obj.data(1), obj.data(2), obj.data(3));
        end

        function ret = last(obj)
            ret = obj.data;
        end

        function ret = penult(obj)
            ret = zeros(1, size(obj.data, 2));
        end
    end
end