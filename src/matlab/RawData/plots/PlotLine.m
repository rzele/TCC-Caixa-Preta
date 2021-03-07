classdef PlotLine < CustomPlot
    % Esta classe é do tipo 'plot' implementa metódos que facilitam a criação
    % de um gráfico em linha
    %
    % Podendo ser injetado na classe de 'render' para ajuste de como
    % este gráfico ficará posicionado na tela, e controle de quado será atualizado
    
    properties
        series = {}
        
        p_title
        p_xlabel
        p_ylabel
        s_legend
        sources_color
    end
    
    methods
        function obj = PlotLine(p_title, p_xlabel, p_ylabel, s_legend, sources_color)
            obj.p_title = p_title;
            obj.p_xlabel = p_xlabel;
            obj.p_ylabel = p_ylabel;
            obj.s_legend = s_legend;
            obj.sources_color = sources_color;
        end

        function createPlot(obj, n_rows, n_columns, grid)
            obj.grid = grid;
            obj.n_rows = n_rows;
            obj.n_columns = n_columns;

            obj.subplot = subplot(obj.n_rows, obj.n_columns, obj.grid);
            grid;
            hold on
            for i = 1:length(obj.sources_color)
                obj.series{i} = plot(0, 'Color', obj.sources_color{i});
            end
            hold off

            title(obj.p_title)
            xlabel(obj.p_xlabel);
            ylabel(obj.p_ylabel);
            legend(obj.s_legend);
            xlim([0 4000]);
        end

        function setSeries(obj, source)
            for i = 1:size(source, 1)
                obj.series{i}.YData = source(i, :);
            end
        end

        function update(obj)
        end
    end
    
end

