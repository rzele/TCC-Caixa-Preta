classdef CompareTilts < CommonsLine
    % Implementa um gráfico que compara pitch, row ou yaw de algum método de estimativa dado
    % Este módulo retorna ao final alguns dados de análise como desvio padrão

    properties
        data_type       % Tipo de dado de inclinação a ser comparado, pode ser 'pitch', 'roll' ou 'yaw'
        data_order      % Ordem no array do gráfico de tilt passado que se encontra o dado
                        % dependendo do tipo definido. E.g.: roll é o primeiro no array retornando pelo .last()

        error_arr

        % Chart dependences obj
        base_line_chart
        others_charts_arr
    end

    methods (Static)
        % Use esta função para terceirizar a criação individual de cada comparação de tilt.
        % Ela recebe os mesmo parâmetros da classe normal, com exceção do 'data_type'
        % e retorna uma struct com os gráficos de comparação de roll, pitch, yaw e um de erro
        function ret = factory_row_pitch_yaw(w_size, base_line_chart, others_charts_arr)
            compare_rolls = CompareTilts('roll', w_size, base_line_chart, others_charts_arr);
            compare_pitchs = CompareTilts('pitch', w_size, base_line_chart, others_charts_arr);
            compare_yaws = CompareTilts('yaw', w_size, base_line_chart, others_charts_arr);
            errors = CompareTilts('error', w_size, base_line_chart, others_charts_arr);

            ret = struct('roll', compare_rolls, 'pitch', compare_pitchs, 'yaw', compare_yaws, 'errors', errors);
        end
    end

    methods
        function obj = CompareTilts(data_type, w_size, base_line_chart, others_charts_arr)
            if strcmp(data_type, 'error')
                p_title = sprintf('Erro da estimativa');
                p_label = 'erro';
            else
                p_title = sprintf('Compara %s', data_type);
                p_label = 'graus';
            end

            s_legend  = {'baseline'};
            for i=1:length(others_charts_arr)
                s_legend{i+1} = class(others_charts_arr(i));
            end

            % Definido somente 7 cores. Muitas linhas no mesmo gráfico pode ficar difícil de entender
            sources_color = {[0,0,0], [0 0.4470 0.7410], [0.8500 0.3250 0.0980], [0.9290 0.6940 0.1250], [0.4940 0.1840 0.5560], [0.4660 0.6740 0.1880], [0.6350 0.0780 0.1840]};

            obj = obj@CommonsLine(...
                p_title, ...                        % p_title
                'Amostra', ...                      % p_xlabel
                p_label, ...                        % p_ylabel
                s_legend, ...                       % s_legend
                sources_color, ...                  % sources_color
                w_size);

            obj.w_size = w_size;
            obj.data_type = data_type;
            obj.data = zeros(w_size, 1 + length(others_charts_arr));
            obj.error_arr = zeros(1, length(others_charts_arr));

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
            elseif strcmp(data_type, 'error')
                obj.data_order = -1;
            else
                error('Invalid data_type, must be roll, pitch or yaw');
            end
        end
        
        function calculate(obj, mpu_new_data, baselines_new_data, n_sample)
            %% Verifica se já calculou essa amostra
            if obj.has_calculated_this_sample(n_sample)
                return
            end
            
            %% Obtem o valor de outros charts ao qual este é dependente
            % Obtem data do baseline
            obj.base_line_chart.calculate(mpu_new_data, baselines_new_data, n_sample);
            base_line_data = obj.base_line_chart.last();
            new_data = base_line_data';

            % Obtem data dos outros gráficos
            for i=1:length(obj.others_charts_arr)
                obj.others_charts_arr(i).calculate(mpu_new_data, baselines_new_data, n_sample);
                chart_data = obj.others_charts_arr(i).last();
                new_data = horzcat(new_data, chart_data');
            end

            %% Calcula o valor p/ a próxima amostra
            % Plota um dos angulos selecionado, se não foi selecionado para plotar o erro
            % Note que data(:, 1) é sempre o baseline
            if obj.data_order ~= -1
                obj.data = vertcat(obj.data(2:obj.w_size, :), new_data(obj.data_order,:));
            end

            %% Calcula dados de estatistica
            obj.calculate_error(new_data(:,1), new_data(:, 2:size(new_data, 2)));
        end

        function calculate_error(obj, baseline, datas_arr)
            % Calcula a diferença da matriz gerada pela estimativa
            % com a matriz esperada pelo baseline,
            % para assim verificar quão próximo uma estimativa ficou do baseline.
            % 
            % OBS: Compara ângulos diretamente não é eficaz, uma vez que (0, 89, -69) e (70, 89, 0)
            % são duas rotações que são mais próximas do que (0, 89, -69) e (0, 89, -65)
            new_data_err = [0];
            baseline_rot = ang2rotZYX(baseline(1), baseline(2), baseline(3));

            for i=1:size(datas_arr, 2)
                cur_estimative = datas_arr(:, i);
                estimated_rot = ang2rotZYX(cur_estimative(1), cur_estimative(2), cur_estimative(3));
                diff = sum(sum(abs(baseline_rot - estimated_rot)));
                new_data_err = [new_data_err, diff];
                obj.error_arr(i) = obj.error_arr(i) + diff;
            end

            % Se foi selecionado para plotar o erro, adiciona o erro de cada amostra estimativa
            % isso ao obj.data para ser plotado o erro ao longo do tempo
            if obj.data_order == -1
                obj.data = [obj.data(2:obj.w_size, :); new_data_err];
            end
        end

        function on_delete(obj)
            % Este método sobrescreve o método das classes plot
            % É chamado quando o render vai finalizar
            % Aqui será utilizado para exibir as estatísticas calculadas
            if obj.data_order == -1
                fprintf('[Compara %s]\n', obj.data_type);
                for i=1:length(obj.others_charts_arr)
                    err = obj.error_arr(i) / obj.last_sample;
                    fprintf('error - %s: %.5f\n', class(obj.others_charts_arr(i)), err);
                end
                fprintf('\n');
            end
        end
    end
end