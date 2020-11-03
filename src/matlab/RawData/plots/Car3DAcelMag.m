classdef Car3DAcelMag < TemplateCar3D
    properties
    end

    methods
        function obj = Car3DAcelMag()
            obj.name = 'Car3DAcelMag';
        end

        function obj = initialize(obj, fig)
            obj.my_plot = fig.setItemType(obj, obj.name, 'plot3dcar');
            obj.my_plot.configPlot('Rotação 3D usando acel e mag');
            obj.data = zeros(1, 3);
        end
    end
end