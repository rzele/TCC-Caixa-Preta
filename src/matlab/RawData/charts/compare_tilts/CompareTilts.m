classdef CompareTilts < CommonsLine
    % Implementa um gr�fico que compara pitch, row ou yaw de algum m�todo de estimativa dado
    % Este m�dulo retorna ao final alguns dados de an�lise como desvio padr�o

    properties
        data_type       % Tipo de dado de inclina��o a ser comparado, pode ser 'pitch', 'roll' ou 'yaw'
        data_order      % Ordem no array do gr�fico de tilt passado que se encontra o dado
                        % dependendo do tipo definido. E.g.: roll � o primeiro no array retornando pelo .last()

        mse_arr
        variance_arr

        % Chart dependences obj
        base_line_chart
        others_charts_arr
    end

    methods (Static)
        % Use esta fun��o para terceirizar a cria��o individual de cada compara��o de tilt.
        % Ela recebe os mesmo par�metros da classe normal, com exce��o do 'data_type'
        % e retorna uma struct com os gr�ficos de compara��o de roll, pitch e yaw
        function ret = factory_row_pitch_yaw(w_size, base_line_chart, others_charts_arr)
            compare_rolls = CompareTilts('roll', w_size, base_line_chart, others_charts_arr);
            compare_pitchs = CompareTilts('pitch', w_size, base_line_chart, others_charts_arr);
            compare_yaws = CompareTilts('yaw', w_size, base_line_chart, others_charts_arr);

            ret = struct('roll', compare_rolls, 'pitch', compare_pitchs, 'yaw', compare_yaws);
        end
    end

    methods
        function obj = CompareTilts(data_type, w_size, base_line_chart, others_charts_arr)
            p_title = sprintf('Compara %s', data_type);

            s_legend  = {'baseline'};
            for i=1:length(others_charts_arr)
                s_legend{i+1} = class(others_charts_arr(i));
            end

            % Definido somente 7 cores. Muitas linhas no mesmo gr�fico pode ficar dif�cil de entender
            sources_color = {[0,0,0], [0 0.4470 0.7410], [0.8500 0.3250 0.0980], [0.9290 0.6940 0.1250], [0.4940 0.1840 0.5560], [0.4660 0.6740 0.1880], [0.6350 0.0780 0.1840]};

            obj = obj@CommonsLine(...
                p_title, ...                        % p_title
                'Amostra', ...                      % p_xlabel
                'graus', ...                        % p_ylabel
                s_legend, ...                       % s_legend
                sources_color, ...                  % sources_color
                w_size);

            obj.w_size = w_size;
            obj.data_type = data_type;
            obj.data = zeros(w_size, 1 + length(others_charts_arr));
            obj.mse_arr = zeros(1, length(others_charts_arr));
            obj.variance_arr = zeros(1, length(others_charts_arr));

            % Chart dependences
            obj.base_line_chart = base_line_chart;
            obj.others_charts_arr = others_charts_arr;

            % Define onde do methodo .last() no outro gr�fico se encontra o dado desse tipo
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
        
        function calculate(obj, mpu_new_data, baselines_new_data, n_sample)
            %% Verifica se j� calculou essa amostra
            if obj.has_calculated_this_sample(n_sample)
                return
            end
            
            %% Obtem o valor de outros charts ao qual este � dependente
            % Obtem data do baseline
            obj.base_line_chart.calculate(mpu_new_data, baselines_new_data, n_sample);
            base_line_data = obj.base_line_chart.last();
            new_data = [base_line_data(obj.data_order)];

            % Obtem data dos outros gr�ficos
            for i=1:length(obj.others_charts_arr)
                obj.others_charts_arr(i).calculate(mpu_new_data, baselines_new_data, n_sample);
                chart_data = obj.others_charts_arr(i).last();
                new_data = [new_data, chart_data(obj.data_order)];
            end

            %% Calcula o valor p/ a pr�xima amostra
            % note que data(:, 1) � sempre o baseline
            obj.data = [obj.data(2:obj.w_size, :); new_data];

            %% Calcula dados de estatistica
            obj.calculate_mse(new_data(1), new_data(2:length(new_data)));
            obj.caculate_variance(new_data(1), new_data(2:length(new_data)));
        end

        function calculate_mse(obj, baseline, datas_arr)
            % Calcula o erro m�dio quadr�tico das entradas, dado um baseline.
            % Como este c�lculo � dado intera��o a intera��o, sem salvar todos valores lidos,
            % ser� realizado aqui somente at� a etapa de soma, a divis�o pelo n� de amostras
            % ser� executado somente no momento de exibi��o
            % 
            % O c�lculo MSE indica qu�o pr�ximo (em m�dia) do baseline esta um dado m�todo de estimativa
            for i=1:length(datas_arr)
                cur_mse = obj.mse_arr(i);
                cur_data = datas_arr(i);
                diff = pow2(baseline - cur_data);
                obj.mse_arr(i) = cur_mse + diff;
            end
        end

        function caculate_variance(obj, baseline, datas_arr)
            % Calcula a vari�ncia das entradas, dado um baseline.
            % Seu funcionamento � semelhante ao calculate_mse
            % 
            % O c�lculo da Vari�ncia indica qu�o disperso est� o m�todo de estimativa
            % comparado ao baseline
        end

        function on_delete(obj)
            % Este m�todo sobrescreve o m�todo das classes plot
            % � chamado quando o render vai finalizar
            % Aqui ser� utilizado para exibir as estat�sticas calculadas
            fprintf('[Compara %s]\n', obj.data_type);
            for i=1:length(obj.others_charts_arr)
                mse = obj.mse_arr(i) / obj.last_sample;
                fprintf('mse - %s: %.5f\n', class(obj.others_charts_arr(i)), mse);
            end
            fprintf('\n');
        end
    end
end