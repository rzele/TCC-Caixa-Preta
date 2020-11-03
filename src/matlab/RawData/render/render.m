classdef Render < handle
    %PLOT Summary of this class goes here
    %   Detailed explanation goes here
    
    properties
        fig
        time
        freq_render
        setted_objects_name = {}
        plots = CustomPlot.empty()
        layout = struct()
        layout_default_struct = struct(    ...
            'subplot', [],                 ...
            'grid', [],                    ...
            'series', []                   ...
        )
        grid_n_rows = 5
        grid_n_columns = 6
        count_render_times = 0
        time_rendering = 0;
    end
    
    methods
        % Constructor: inicializa o plot seguindo o layout, e inicializa a
        % contagem do render
        function obj = Render(freq, layout)
            addpath('quaternion_library');      % include quaternion library
            aux_setted_objects_name = struct();

            obj.grid_n_rows = length(layout(:,1));
            obj.grid_n_columns = length(layout(1,:));
            obj.freq_render = freq;
            obj.time = now;

            % Define o grid de cada objeto no layout em fun��o de linha e colunas do grid
            for row = 1:obj.grid_n_rows
                for col = 1:obj.grid_n_columns
                    obj_name = layout{row, col};

                    if isobject(obj_name)
                        obj_name = obj_name.name;
                    end

                    if ~strcmp(obj_name,'')
                        % Istancia a estrutura do objeto do layout se ainda não iniciada
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
        end

        function new_plot = setItemType(obj, obj_instance, obj_name, type_name)
            if ~isfield(obj.layout, obj_name)
                new_plot = EmptyPlot();
                return
            end

            if strcmp(type_name, 'plotline')
                new_plot = PlotLine(obj.grid_n_rows, obj.grid_n_columns, obj.layout.(obj_name).grid);
            elseif strcmp(type_name, 'plot3dcar')
                new_plot = Plot3DCar(obj.grid_n_rows, obj.grid_n_columns, obj.layout.(obj_name).grid);
            elseif strcmp(type_name, 'plot3dline')
                new_plot = Plot3DLine(obj.grid_n_rows, obj.grid_n_columns, obj.layout.(obj_name).grid);
            elseif strcmp(type_name, 'plotcompass')
                new_plot = PlotCompass(obj.grid_n_rows, obj.grid_n_columns, obj.layout.(obj_name).grid);
            else
                error('type_name is invalid');
            end

            obj.layout.(obj_name).instance = obj_instance;

            obj.plots(length(obj.plots)+1) = new_plot;
        end

        % Tenta redesenhar o plot, se deu o tempo da frequencia
        function try_render(obj)
            % Re plota
            if (now-obj.time)*100000 > 1/obj.freq_render
                obj.count_render_times = obj.count_render_times + 1;
                t1 = tic;

                obj.update_all();

                refreshdata
                drawnow
                obj.time = now;
                
                obj.time_rendering = obj.time_rendering + toc(t1);
            end
        end

        function update_all(obj)
            for i = 1:length(obj.setted_objects_name)
                obj_name = obj.setted_objects_name{i};
                obj.layout.(obj_name).instance.update();
            end
        end

        % Rendereza mesmo se nã0o deu o tempo da frequencia
        function force_render(obj)
            figure(1)
            obj.update_all();
            refreshdata
            drawnow
            obj.time = now;
        end

        % Linka todos os eixos, so pode ser executado quando n?o for mais atualizar os gr?ficos
        function linkAllAxes(obj)
            axis = [];

            for i = 1:length(obj.setted_objects_name)
                obj_name = cell2mat(obj.setted_objects_name(i));
                axis = [axis obj.layout.(obj_name).subplot];
            end

            linkaxes(axis, 'x');
        end

        function delete(obj)
            fprintf('Tempo m�dio da fun��o de renderiza��o: %fs\n', obj.time_rendering / obj.count_render_times);
        end
    end
end