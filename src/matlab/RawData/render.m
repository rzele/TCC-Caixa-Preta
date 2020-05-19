classdef render < handle
    %PLOT Summary of this class goes here
    %   Detailed explanation goes here
    
    properties
        fig
        time
        freq_render
        setted_objects_name = {}
        layout = struct()
        layout_default_struct = struct(    ...
            'subplot', [],                   ...
            'grid', [],                    ...
            'plots', struct (              ...
                'x', [],                   ...
                'y', [],                   ...
                'z', []                    ...
            )                              ...
        )
        grid_n_rows = 5
        grid_n_columns = 6
    end
    
    methods
        % Constructor: inicializa o plot seguindo o layout, e inicializa a
        % contagem do render
        function obj = render(freq, layout)
            aux_setted_objects_name = struct();

            obj.grid_n_rows = length(layout(:,1));
            obj.grid_n_columns = length(layout);
            obj.freq_render = freq;
            obj.time = now;

            % Define o grid de cada objeto no layout em função de linha e colunas do grid
            for row = 1:obj.grid_n_rows
                for col = 1:obj.grid_n_columns
                    obj_name = cell2mat(layout(row, col));

                    if ~strcmp(obj_name,'')
                        % Istancia a estrutura do objeto do layout se ainda n?o iniciada
                        if ~isfield(obj.layout, obj_name)
                            obj.layout.(obj_name) = obj.layout_default_struct;
                        end

                        position = ((row - 1) * obj.grid_n_columns) + col;
                        obj.layout.(obj_name).grid = [obj.layout.(obj_name).grid position];
                        aux_setted_objects_name.(obj_name) = [];
                    end
                end
            end

            obj.setted_objects_name = fieldnames(aux_setted_objects_name);

            obj.fig = figure('units','normalized','outerposition',[0 0 1 1]);

            % Percorre cada objeto do layout definido renderizando o quadro de plot com 3 eixos
            for i = 1:length(obj.setted_objects_name)
                obj_name = cell2mat(obj.setted_objects_name(i));
                obj.layout.(obj_name).subplot = subplot(obj.grid_n_rows, obj.grid_n_columns, obj.layout.(obj_name).grid);
                obj.layout.(obj_name).plots.x = plot(0, 'r');
                grid;
                hold on
                obj.layout.(obj_name).plots.y = plot(0, 'g');
                obj.layout.(obj_name).plots.z = plot(0, 'b');
                hold off
            end
        end

        % Define as propriedades de cada objeto no layout (titulo, legenda...)
        function setProperties(obj, obj_name, p_title, p_xlabel, p_ylabel, p_legend)

            % Verifica se o objeto existe e foi inicializado
            if isfield(obj.layout, obj_name) && ~isempty(obj.layout.(obj_name).subplot)
                subplot(obj.layout.(obj_name).subplot)
                title(p_title)
                xlabel(p_xlabel);
                ylabel(p_ylabel);
                legend(p_legend);
            end
        end
        
        % Define a variavel source de cada eixo
        function setSource(obj, obj_name, x_source_name, y_source_name, z_source_name)
            % Equivalente a fazer p.YDataSource = 'ax'; p.YDataSource = 'ay'; p.YDataSource = 'az' no workspace base
            obj.layout.(obj_name).plots.x.YDataSource = x_source_name;
            obj.layout.(obj_name).plots.y.YDataSource = y_source_name;
            obj.layout.(obj_name).plots.z.YDataSource = z_source_name;
        end

        % Tenta redesenhar o plot, se deu o tempo da frequencia
        function try_render(obj)
            % Re plota
            if (now-obj.time)*100000 > 1/obj.freq_render
                refreshdata
                drawnow
                obj.time = now;
            end
        end

        % Rendereza mesmo se não deu o tempo da frequencia
        function force_render(obj)
            refreshdata
            drawnow
            obj.time = now;
        end
    end
    
end