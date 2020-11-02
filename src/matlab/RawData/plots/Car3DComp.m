classdef Car3DComp < TemplateLine
    properties
    end

    methods
        function obj = Car3DComp()
            obj.name = 'Car3DComp';
        end

        function obj = initialize(obj, fig)
            obj.my_plot = fig.setItemType(obj.name, 'plot3dcar');
            obj.my_plot.configPlot('Rotação 3D usando filtro complementar');
            obj.data = zeros(1, 3);
        end

        function calculate(obj, tilt)
            obj.data = tilt;
        end

        function update(obj)
            obj.my_plot.rotateWithEuler(obj.data(1), obj.data(2), obj.data(3));
        end
    end
end