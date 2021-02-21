classdef TemplateCar3D < Template & Plot3DCar
    properties
    end

    methods
        function obj = TemplateCar3D(p_title)
            obj = obj@Plot3DCar(p_title);
        end

        function calculate(obj, new_data)
            obj.data = new_data;
        end
        
        function ret = last(obj)
            ret = obj.data;
        end

        function ret = penult(obj)
            ret = zeros(1, size(obj.data, 2));
        end
    end
end