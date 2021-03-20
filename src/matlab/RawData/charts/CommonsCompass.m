classdef CommonsCompass <  Template & PlotCompass
    % Esta classe � do tipo 'c�lculo' e do tipo 'plot'
    % Sendo assim, cabe a ela realizar c�lculos e ao mesmo tempo
    % renderizar os dados na tela em 'gr�ficos de linha'.
    % A este tipo hibrido de classe demos o nome de 'chart'
    %
    % Por conta desta classe herdar 'PlotCompass', esta classe (ou qualquer outra que herdar ela)
    % pode ser injetada na classe 'render', para exibir o gr�fico na tela.
    %
    % Sobrescrevendo o m�todo 'update' do 'plotline', quando a 'render' chamar
    % este m�todo avisando que ocorrer� uma atualiza��o na tela,
    % esta classe pode atualizar o plot com os dados atuais c�lculados.
    % Sendo assim, n�o cabe mais a main, gerenciar os c�lculos e seus respectivos plots
    % Tornando cada gr�fico visualizado na tela um m�dulo independente. 

    properties
        w_size
    end

    methods
        function obj = CommonsCompass(p_title)
            obj = obj@PlotCompass(p_title);
            obj.last_sample = 0;
        end

        function on_update(obj)
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