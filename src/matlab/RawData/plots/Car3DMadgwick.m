classdef Car3DMadgwick < TemplateCar3D
    properties
    end

    methods
        function obj = Car3DMadgwick()
            obj.name = 'Car3DMadgwick';
        end

        function obj = initialize(obj, fig)
            obj.my_plot = fig.setItemType(obj.name, 'plot3dcar');
            obj.my_plot.configPlot('Rotação 3D usando quaternions (filtro Madgwick)');
            obj.data = zeros(1, 4);
        end

        function update(obj)
            obj.my_plot.rotateWithQuaternion(obj.data);
        end
    end
end