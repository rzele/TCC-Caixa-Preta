classdef Plot3DLine < CustomPlot
    %PLOT3DLINE Summary of this class goes here
    %   Detailed explanation goes here
    
    properties
        grid = []
        subplot
        n_rows
        n_columns
        series = []
        sources = []
    end
    
    methods
        function obj = Plot3DLine(n_rows, n_columns, grid, obj_name)
            obj.grid = grid;
            obj.n_rows = n_rows;
            obj.n_columns = n_columns;
        end

        % Define as propriedades do objeto no layout (titulo, legenda...)
        function configPlot(obj, p_title)
            obj.subplot = subplot(obj.n_rows, obj.n_columns, obj.grid);
            serie = plot3([0], [0], [0]);

            title(p_title)
            % xlabel('x');
            % ylabel('y');
            % zlabel('z');
        end

        function update(obj, sources)
            % Update the plot data here
        end
    end
    
end

