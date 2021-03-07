classdef CompareTilts < CommonsLine
    % Implementa um gráfico que compara pitch, row ou yaw de algum método de estimativa dado
    % Este módulo retorna ao final alguns dados de análise como desvio padrão

    properties
        data_type       % Tipo de dado de inclinação a ser comparado, pode ser 'pitch', 'roll' ou 'yaw'
        data_order      % Ordem no array do gráfico de tilt passado que se encontra o dado
                        % dependendo do tipo definido. E.g.: roll é o primeiro no array retornando pelo .last()

        % Chart dependences obj
        base_line_chart
        others_charts_arr
    end

    methods
        function obj = CompareTilts(data_type, w_size, base_line_chart, others_charts_arr)
            p_title = sprintf('Compara %s', data_type);

            s_legend  = {'baseline'};
            for i=1:length(others_charts_arr)
                s_legend{i+1} = class(others_charts_arr(i));
            end

            % Definido somente 7 cores. Muitas linhas no mesmo gráfico pode ficar difícil de entender
            sources_color = {[0,0,0], [0 0.4470 0.7410], [0.8500 0.3250 0.0980], [0.9290 0.6940 0.1250], [0.4940 0.1840 0.5560], [0.4660 0.6740 0.1880], [0.6350 0.0780 0.1840]};

            obj = obj@CommonsLine(...
                p_title, ...                        % p_title
                'Amostra', ...                      % p_xlabel
                'graus', ...                        % p_ylabel
                s_legend, ...                       % s_legend
                sources_color);                     % sources_color

            obj.w_size = w_size;
            obj.data_type = data_type;
            obj.data = zeros(w_size, 1 + length(others_charts_arr));

            % Chart dependences
            obj.base_line_chart = base_line_chart;
            obj.others_charts_arr = others_charts_arr;

            % Define onde do methodo .last() no outro gráfico se encontra o dado desse tipo
            if strcmp(data_type, 'roll')
                obj.data_order = 1;
            elseif strcmp(data_type, 'pitch')
                obj.data_order = 2;
            elseif strcmp(data_type, 'yaw')
                obj.data_order = 3;
            else
                error('Invalid data_type, must be roll, pitch or yaw');
            end
        end
        
        function calculate(obj, mpu_new_data, n_sample)
            %% Verifica se já calculou essa amostra
            if obj.has_calculated_this_sample(n_sample)
                return
            end
            
            %% Obtem o valor de outros charts ao qual este é dependente
            % Obtem data do baseline
            obj.base_line_chart.calculate(mpu_new_data, n_sample);
            base_line_data = obj.base_line_chart.last();
            new_data = [base_line_data(obj.data_order)];

            % Obtem data dos outros gráficos
            for i=1:length(obj.others_charts_arr)
                obj.others_charts_arr(i).calculate(mpu_new_data, n_sample);
                chart_data = obj.others_charts_arr(i).last();
                new_data = [new_data, chart_data(obj.data_order)];
            end

            %% Calcula o valor p/ a próxima amostra
            % note que data(:, 1) é sempre o baseline
            obj.data = [obj.data(2:obj.w_size, :); new_data];
        end
    end
end