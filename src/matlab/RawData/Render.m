classdef Render < handle
    %PLOT Summary of this class goes here
    %   Detailed explanation goes here
    
    properties
        fig
        time
        freq_render
        setted_objects_id = {}
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
            aux_setted_objects_id = struct();

            obj.grid_n_rows = length(layout(:,1));
            obj.grid_n_columns = length(layout(1,:));
            obj.freq_render = freq;
            obj.time = now;

            % Define o grid de cada objeto no layout em funï¿½ï¿½o de linha e colunas do grid
            for row = 1:obj.grid_n_rows
                for col = 1:obj.grid_n_columns
                    plot_obj = layout{row, col};

                    if ~isobject(plot_obj)
                        continue;
                    end

                    % Istancia a estrutura do objeto do layout se ainda não iniciada
                    if ~isfield(obj.layout, plot_obj.id)
                        obj.layout.(plot_obj.id) = obj.layout_default_struct;
                        obj.plots(length(obj.plots)+1) = plot_obj;
                        obj.setted_objects_id = [obj.setted_objects_id plot_obj.id];
                    end

                    position = ((row - 1) * obj.grid_n_columns) + col;
                    obj.layout.(plot_obj.id).grid = [obj.layout.(plot_obj.id).grid position];
                end
            end

            obj.fig = figure('name', 'Dashboard', 'units','normalized','outerposition',[0 0 1 1]);
            
            % Cria cada plot na tela
            for i = 1:length(obj.plots)
                plot_id = obj.plots(i).id;
                obj.plots(i).createPlot(obj.grid_n_rows, obj.grid_n_columns, obj.layout.(plot_id).grid);
            end
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
            for i = 1:length(obj.plots)
                obj.plots(i).on_update();
            end
        end

        % Rendereza mesmo se que não tenha dado o tempo da frequencia
        function force_render(obj)
            figure(obj.fig)
            obj.update_all();
            refreshdata
            drawnow
            obj.time = now;
        end

        function delete(obj)
            % Dispara esse evento para cada gráfico realizar sua finalização se necessário
            for i=1:length(obj.plots)
                obj.plots(i).on_delete();
            end
            fprintf('Tempo médio da função de renderização: %fs\n', obj.time_rendering / obj.count_render_times);
        end
    end
end