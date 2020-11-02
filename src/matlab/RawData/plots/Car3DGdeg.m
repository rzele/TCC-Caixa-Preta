classdef Car3DGdeg < TemplateCar3D
    properties
    end

    methods
        function obj = Car3DGdeg()
            obj.name = 'Car3DGdeg';
        end

        function obj = initialize(obj, fig)
            obj.my_plot = fig.setItemType(obj.name, 'plot3dcar');
            obj.my_plot.configPlot('Rotação 3D usando Posição angular relativa');
            obj.data = zeros(1, 3);
        end
    end
end