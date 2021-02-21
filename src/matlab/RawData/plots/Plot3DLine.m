classdef Plot3DLine < CustomPlot
    %PLOT3DLINE Summary of this class goes here
    %   Detailed explanation goes here
    
    properties
        series = []

        p_title
    end
    
    methods
        function obj = Plot3DLine(p_title)
            obj.p_title = p_title;
        end

        % Define as propriedades do objeto no layout (titulo, legenda...)
        function createPlot(obj, n_rows, n_columns, grid)
            obj.grid = grid;
            obj.n_rows = n_rows;
            obj.n_columns = n_columns;

            obj.subplot = subplot(obj.n_rows, obj.n_columns, obj.grid);
            serie = plot3([0], [0], [0]);

            title(obj.p_title)
            % xlabel('x');
            % ylabel('y');
            % zlabel('z');
        end

        function update(obj)
        end
    end
    
end

