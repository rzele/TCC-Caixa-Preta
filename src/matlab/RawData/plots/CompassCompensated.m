classdef CompassCompensated < TemplateLine
    properties
    end

    methods
        function obj = CompassCompensated()
            obj.name = 'CompassCompensated';
        end

        function obj = initialize(obj, fig)
            obj.my_plot = fig.setItemType(obj.name, 'plotcompass');
            obj.my_plot.configPlot('Magnetic Heading COM compensação de tilt');
            obj.data = 0;
        end

        function calculate(obj, yaw)
            obj.data = yaw;
        end

        function update(obj)
            obj.my_plot.rotateCompass(obj.data);
        end
    end
end