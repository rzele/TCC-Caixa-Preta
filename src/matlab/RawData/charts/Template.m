classdef Template < handle
    % Todas as classes que herdarem este superclasse deve implementar
    % os métodos abaixo.
    %
    % Esta classe server como 'Abstract', defininado a interface padrão
    % que uma classe de 'cálculo' deve ter
    
    properties
        data
    end

    methods
        function obj = Template()
        end

        function calculate(obj)
        end

        function ret = last(obj)
        end

        function ret = penult(obj)
        end
    end
end