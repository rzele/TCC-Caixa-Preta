classdef CustomPlot < matlab.mixin.Heterogeneous & handle
    % Todas as classes que herdarem este superclasse deve implementar
    % os métodos abaixo.
    %
    % Esta classe server como 'Abstract', defininado a interface padrão
    % que uma classe de 'plot' deve ter, para poder ser injetada na classe 'render'
    %
    % matlab.mixin.Heterogeneous é uma superclasse que permite
    % que a classe 'CustomPlot' ou qualquer outra que a herdar
    % seja agrupada em array. e.x:
    % inst1 = CustomPlot();
    % inst2 = CustomPlot();
    % inst_herdada = ClasseQueHerodouACustomPlot();
    % arr = [inst1, inst2, inst_herdada];

    properties
        id
        grid = []
        subplot
        n_rows
        n_columns
    end

    methods
        function obj = CustomPlot()
            uuid = char(java.util.UUID.randomUUID.toString);
            obj.id = strcat('p', regexprep(uuid, '-', ''));
        end

        function createPlot(obj)
        end

        function update(obj)
        end
    end
end