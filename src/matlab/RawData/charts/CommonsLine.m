classdef CommonsLine <  Template & PlotLine
    % Esta classe é do tipo 'cálculo' e do tipo 'plot'
    % Sendo assim, cabe a ela realizar cálculos e ao mesmo tempo
    % renderizar os dados na tela em 'gráficos de linha'.
    % A este tipo hibrido de classe demos o nome de 'chart'
    %
    % Por conta desta classe herdar 'PlotLine', esta classe (ou qualquer outra que herdar ela)
    % pode ser injetada na classe 'render', para exibir o gráfico na tela.
    %
    % Sobrescrevendo o método 'update' do 'plotline', quando a 'render' chamar
    % este método avisando que ocorrerá uma atualização na tela,
    % esta classe pode atualizar o plot com os dados atuais cálculados.
    % Sendo assim, não cabe mais a main, gerenciar os cálculos e seus respectivos plots
    % Tornando cada gráfico visualizado na tela um módulo independente.
    %
    % NOTA: Esta class tem a função de implementar algumas das funções do template.
    % Uma vez que essas funções implementadas seram iguais para quase todos os charts do tipo,
    % evitando assim escrever todas as vez funções como por exemplo: 'update', 'last', 'penult'

    properties
        w_size
    end
    
    methods
        function obj = CommonsLine(p_title, p_xlabel, p_ylabel, s_legend, sources_color, win_size)
            obj = obj@PlotLine(p_title, p_xlabel, p_ylabel, s_legend, sources_color, win_size);
            obj.last_sample = 0;
            obj.time = 0;
        end

        function on_update(obj)
            dataTransposed = obj.data';
            obj.setSeries(dataTransposed);
        end

        function ret = last(obj)
            ret = obj.data(obj.w_size, :);
        end

        function ret = penult(obj)
            ret = obj.data(obj.w_size-1, :);
        end

        function total = get_time_calculating(obj)
            total = 0;

            % tempo médio das dependências executarem
            fn = fieldnames(obj.dependencies);
            for i = 1:length(fn)
                total = total + obj.dependencies.(fn{i}).get_time_calculating();
            end

            % tempo médio total
            total = total + (obj.time / obj.last_sample);
        end

        function ret = has_calculated_this_sample(obj, n_sample)
            if obj.last_sample == n_sample
                ret = true;
            else
                obj.last_sample = n_sample;
                ret = false;
            end
        end

        function on_delete(obj)
            fprintf('[%s]\n', class(obj));
            fprintf('exec_t: %fs\n', obj.get_time_calculating());
            fprintf('\n');
        end
    end
end