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
        function setProperties(obj, p_title, p_xlabel, p_ylabel, s_legend)
            % Verifica se o objeto existe e foi inicializado
            if ~isempty(obj.subplot)
                subplot(obj.subplot)
                title(p_title)
                xlabel(p_xlabel);
                ylabel(p_ylabel);
                % legend(s_legend);
                xlim([0 4000]);
            else
                error('Nao foi inicializado este objeto ainda, defina as fontes usando a funcao setSource.')
            end
        end

        % Define a variavel source de cada eixo
        function setSource(obj, sources_name, sources_color)
            obj.subplot = subplot(obj.n_rows, obj.n_columns, obj.grid);
            grid;
            hold on
            for i = 1:length(sources_name)
                obj.series{i} = plot(0, sources_color{i});
                obj.series{i}.YDataSource = cell2mat(sources_name(i));
            end
            hold off
        end

        function update(obj, source)
            % for i = 1:length(source)
            %     obj.series{i}.YData = source{i};
            % end
        end
    end
    
end

