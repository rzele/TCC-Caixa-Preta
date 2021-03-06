classdef Car3DAcelMag < TemplateCar3D
    properties
    end

    methods
        function obj = Car3DAcelMag()
            obj = obj@TemplateCar3D('Rotação 3D usando acel e mag'); % p_title
            obj.data = zeros(1, 3);
        end

        function update(obj)
            obj.rotateWithEuler(obj.data(1), obj.data(2), obj.data(3));
        end
    end
end