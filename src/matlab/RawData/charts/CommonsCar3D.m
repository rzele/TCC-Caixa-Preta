classdef CommonsCar3D < Template & Plot3DCar
    properties
    end

    methods
        function obj = CommonsCar3D(p_title)
            obj = obj@Plot3DCar(p_title);
            obj.last_sample = 0;
        end
        
        function ret = last(obj)
            ret = obj.data;
        end

        function ret = penult(obj)
            ret = zeros(1, size(obj.data, 2));
        end

        function ret = has_calculated_this_sample(obj, n_sample)
            if obj.last_sample == n_sample
                ret = true;
            else
                obj.last_sample = n_sample;
                ret = false;
            end
        end
    end
end