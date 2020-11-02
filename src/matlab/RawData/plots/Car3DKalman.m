classdef Car3DKalman < TemplateCar3D
    properties
    end

    methods
        function obj = Car3DKalman()
            obj.name = 'Car3DKalman';
        end

        function obj = initialize(obj, fig)
            obj.my_plot = fig.setItemType(obj.name, 'plot3dcar');
            obj.my_plot.configPlot('Rotação 3D usando filtro de kalman');
            obj.data = zeros(1, 3);
        end
    end
end