classdef Car3DMadgwick < TemplateCar3D
    properties
    end

    methods
        function obj = Car3DMadgwick()
            obj = obj@TemplateCar3D('Rotação 3D usando quaternions (filtro Madgwick)'); % p_title
            obj.data = zeros(1, 4);
        end

        function update(obj)
            obj.rotateWithQuaternion(obj.data);
        end
    end
end