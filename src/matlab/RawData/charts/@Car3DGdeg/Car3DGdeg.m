classdef Car3DGdeg < TemplateCar3D
    properties
    end

    methods
        function obj = Car3DGdeg()
            obj = obj@TemplateCar3D('Rotação 3D usando Posição angular relativa'); % p_title
        end

        function obj = initialize(obj, fig)
            obj.data = zeros(1, 3);
        end
        
        function update(obj)
            obj.rotateWithEuler(obj.data(1), obj.data(2), obj.data(3));
        end
    end
end