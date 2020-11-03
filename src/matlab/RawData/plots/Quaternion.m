classdef Quaternion < TemplateLine
    properties
    end

    methods
        function obj = Quaternion()
            obj.name = 'Quaternion';
        end

        function obj = initialize(obj, fig, w_size)
            obj.w_size = w_size;
            obj.my_plot = fig.setItemType(obj, obj.name, 'plotline');
            obj.my_plot.configPlot('Quaterions do filtro de Madgwick', 'Amostra', 'val', {'q1', 'q2', 'q3', 'q4'}, {'r', 'g', 'b', 'y'});
            obj.data = zeros(w_size, 4);
        end

        function calculate(obj, q)
            obj.data = [obj.data(2:obj.w_size, :); q];
        end
    end
end