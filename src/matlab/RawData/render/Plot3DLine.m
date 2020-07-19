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
        function setProperties(obj, p_title)
            % Verifica se o objeto existe e foi inicializado
            if ~isempty(obj.subplot)
                subplot(obj.subplot)
                title(p_title)
                % xlabel('x');
                % ylabel('y');
                % zlabel('z');
            else
                error('Nao foi inicializado este objeto ainda, defina as fontes usando a funcao setSource.')
            end
        end

        % Define a variavel source de cada eixo
        function setSource(obj, sources_name)
            if length(sources_name) ~= 3
                error('Missing sources. check if you are passing 3 sources in order X,Y,Z, an they are defined in a script file')
            end

            obj.subplot = subplot(obj.n_rows, obj.n_columns, obj.grid);

            serie = plot3([0], [0], [0]);
            serie.XDataSource = cell2mat(sources_name(1));
            serie.YDataSource = cell2mat(sources_name(2));
            serie.ZDataSource = cell2mat(sources_name(3));
        end
    end
    
end

