classdef TemplateCompass <  Template & PlotCompass
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

    properties
        w_size
        last_sample=0
    end

    methods
        function obj = TemplateCompass(p_title)
            obj = obj@PlotCompass(p_title);
        end

        function update(obj)
            obj.rotateCompass(obj.data);
        end

        function ret = last(obj)
            ret = obj.data(obj.w_size, :);
        end

        function ret = penult(obj)
            ret = obj.data(obj.w_size-1, :);
        end

        function ret = has_calculated_this_sample(obj, n_sample)
            if obj.last_sample == n_sample
                ret = true;
            else
                obj.last_sample = n_sample;
                ret = false;
            end
        end
    end
end