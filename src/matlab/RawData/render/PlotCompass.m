classdef PlotCompass < CustomPlot
    properties
        grid = []
        subplot
        n_rows
        n_columns
        compass
        sources = []
    end
    
    methods
        function obj = PlotCompass(n_rows, n_columns, grid, obj_name)
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
            else
                error('Nao foi inicializado este objeto ainda, defina as fontes usando a funcao setSource.')
            end
        end

        % Define o plot polar p/ ficar parecido com um compass
        function setCompass(obj)
            obj.subplot = subplot(obj.n_rows, obj.n_columns, obj.grid);

            obj.compass = polarplot([0,0], [0,1]);
            ax = gca;
            ax.ThetaTick = [0 22.5 45 67.5 90 112.5 135 157.5 180 202.5 225 247.5 270 292.5 315 337.25];
            ax.ThetaTickLabel = {'W', '', 'NW', '', 'N', '', 'NE', '', 'E', '', 'SE', '', 'S', '', 'SW', ''};
            ax.ThetaGrid = 'off';
            ax.RTick = [0, 1];
            ax.RTickLabel = {'', ''};
            ax.LineWidth = 1.5;
            obj.subplot = ax;
        end
        
        function rotateCompass(obj, ang)
            % Rotacionado de 90 p/ visualização do norte ficar p/ cima
            ang_rad = (ang + 90)*pi/180;
            obj.compass.ThetaData = [ang_rad, ang_rad];
        end
    end
    
end

