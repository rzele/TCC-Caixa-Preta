classdef PlotLine < CustomPlot
    %PLOTLINE Summary of this class goes here
    %   Detailed explanation goes here
    
    properties
        grid = []
        subplot
        n_rows
        n_columns
        series = {}
        sources = []
    end
    
    methods
        function obj = PlotLine(n_rows, n_columns, grid, obj_name)
            obj.grid = grid;
            obj.n_rows = n_rows;
            obj.n_columns = n_columns;
        end

        % Define as propriedades do objeto no layout (titulo, legenda...)
        function configPlot(obj, p_title, p_xlabel, p_ylabel, s_legend, sources_color)
            obj.subplot = subplot(obj.n_rows, obj.n_columns, obj.grid);
            grid;
            hold on
            for i = 1:length(sources_color)
                obj.series{i} = plot(0, sources_color{i});
            end
            hold off

            title(p_title)
            xlabel(p_xlabel);
            ylabel(p_ylabel);
            % legend(s_legend);
            xlim([0 4000]);
        end

        function update(obj, source)
            for i = 1:size(source, 1)
                obj.series{i}.YData = source(i, :);
            end
        end
    end
    
end

