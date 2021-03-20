classdef Template < handle
    % Todas as classes que herdarem este superclasse deve implementar
    % os m�todos abaixo.
    %
    % Esta classe server como 'Abstract', defininado a interface padr�o
    % que uma classe de 'c�lculo' deve ter
    
    properties
        data
        time
        last_sample
    end

    methods
        function obj = Template()
        end

        function calculate(obj, data, n_sample)
        end

        function get_time_calculating(obj, data, n_sample)
        end

        function ret = last(obj)
        end

        function ret = penult(obj)
        end

        function ret = has_calculated_this_sample(obj, n_sample)
        end
    end
end