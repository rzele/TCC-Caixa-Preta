classdef AcelWithoutG < TemplateLine
    % Calcula a Aceleração absoluta (relativo a terra) removendo a gravidade
    % Obtem os vetores de aceleração atual rotacionando NO MESMO SENTIDO realizado pelo corpo
    % (uma vez que os queremos os vetores que não rotacionam com o corpo, como a gravidade que sempre aponta p/ baixo)
    % (seria o equivalente a desrotacionar a plataforma e ver para onde os vetores de aceleração apontam agora)
    % e subtraindo a gravidade = 1g do eixo Z
    % Usando os dados do filtro de kalman desrotacionar os vetores
    % Ref do calculo: https://www.nxp.com/docs/en/application-note/AN3461.pdf

    properties
    end

    methods
        function obj = AcelWithoutG(w_size)
            obj = obj@TemplateLine(...
                'Aceleração em g sem gravidade', ...        % p_title
                'Amostra', ...                              % p_xlabel
                'g', ...                                    % p_ylabel
                {'aX', 'aY', 'aZ'}, ...                     % s_legend
                {'r', 'g', 'b'});                           % sources_color

            obj.w_size = w_size;
            obj.data = zeros(w_size, 3);
        end
        
        function calculate(obj, tilt, A)
            new_data = obj.calculate_aceleration_without_gravity(tilt, A);
            obj.data = [obj.data(2:obj.w_size, :); new_data];
        end
    end
end