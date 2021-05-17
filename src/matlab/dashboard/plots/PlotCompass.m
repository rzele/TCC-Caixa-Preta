classdef PlotCompass < CustomPlot
    % Esta classe � do tipo 'plot' implementa met�dos que facilitam a cria��o
    % de um gr�fico de b�ssola
    %
    % Podendo ser injetado na classe de 'render' para ajuste de como
    % este gr�fico ficar� posicionado na tela, e controle de quado ser� atualizado

    properties
        compass
        
        p_title
    end
    
    methods
        function obj = PlotCompass(p_title)
            obj.p_title = p_title;
        end

        function createPlot(obj, n_rows, n_columns, grid)
            obj.grid = grid;
            obj.n_rows = n_rows;
            obj.n_columns = n_columns;

            obj.setCompass();
            title(obj.p_title)
        end

        % Define o plot polar p/ ficar parecido com um compass
        function setCompass(obj)
            obj.subplot = subplot(obj.n_rows, obj.n_columns, obj.grid);

            obj.compass = polarplot([0,0], [0,1]);
            ax = gca;
            ax.ThetaTick = [0 22.5 45 67.5 90 112.5 135 157.5 180 202.5 225 247.5 270 292.5 315 337.25];
            ax.ThetaTickLabel = {'E', '', 'NE', '', 'N', '', 'NW', '', 'W', '', 'SW', '', 'S', '', 'SE', ''};
            ax.ThetaGrid = 'off';
            ax.RTick = [0, 1];
            ax.RTickLabel = {'', ''};
            ax.LineWidth = 1.5;
            obj.subplot = ax;
        end

        function on_update(obj)
        end

        function on_delete(obj)
        end
        
        function rotateCompass(obj, ang)
            % Rotacionado de 90 p/ visualiza��o do norte ficar p/ cima
            ang_rad = (ang + 90)*pi/180;
            obj.compass.ThetaData = [ang_rad, ang_rad];
        end
    end
    
end

