classdef Car3DComp < TemplateCar3D
    properties
    end

    methods
        function obj = Car3DComp()
            obj.name = 'Car3DComp';
        end

        function obj = initialize(obj, fig)
            obj.my_plot = fig.setItemType(obj, obj.name, 'plot3dcar');
            obj.my_plot.configPlot('Rotação 3D usando filtro complementar');
            obj.data = zeros(1, 3);
        end
    end
end