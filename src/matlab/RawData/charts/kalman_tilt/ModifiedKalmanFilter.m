classdef ModifiedKalmanFilter < handle
    %KALMAN Implementa e modifica o filtro de kalman para estimativa de inclinação
    %   OBS: Esta implemantação modifica o filtro de kalman para manter a
    %   subtração (medida - estimativa) dentro do intervalo -180,180.
    %   Esta mofificação é necessário conforme exemplificado mais abaixo!
    %   Este filtro somente funciona para o modelo criado, ou seja,
    %   os valores de A,B,C definidos.
    %   Para utilizar com outro modelo, crie uma cópia e remova os conjuntos de if/else mais abaixo.
    %
    %   Formato padrão de modelagem:
    %   x[k] = A*x[k-1] + B*u[k] + w[k]
    %   y[k] = C*x[k] + v[k]
    %   E[wwt] = Q
    %   E[vvt] = R
    %
    % OBS: A implementação do predic e as operações sumAngle e subAngle,
    % não são da implementação normal do filtro de kalman.
    % Ela é utilizada pois precisamos dos os ângulos de Euler (detalhes na função predict).
    % E que estejam entre -180,180 para não dar problema na subtração no momento de atualização. 
    % E.g.:
    %   Se nossa ultima estimativa foi -180 e a nova medida for 180, a diferença fica
    %   -180 - (-180) = 360
    %   Atualizar a estimativa atual de k*360 e diferente de k*0
    % Ao utilizar soma e subtração de angulos utilizado a função, garantimos o intervalo desejado.
    % Por esses motivos essa implementação é específica para o modelo utilizado do filtro de kalman.

    properties
        xk_minus                % x estimado p/ o instante k
        pk_minus                % p estimado p/ o instante k
        xk_1_minus              % x estimado p/ o instante k-1
        pk_1_minus              % p estimado p/ o instante k-1

        K                       % Matriz de ajuste

        Q                       % Erro do valor de entrada
        R                       % Erro do valor de saída do Model

        A                       % (Matriz dinâmica)
        B                       % (Matriz de entrada)
        C                       % (Matriz de saída)
    end
    
    methods
        function obj = ModifiedKalmanFilter(A,B,C,Q,R)
           obj.A = A;
           obj.B = B;
           obj.C = C;
           obj.Q = Q;
           obj.R = R;

           obj.xk_1_minus = zeros(length(A), 1);
           obj.xk_minus = zeros(length(A), 1);
           obj.pk_1_minus = zeros(length(A));
           obj.pk_minus = zeros(length(A));
        end

        % Realiza a etapa de predição do algoritmo, salvando no objeto os valores preditos, não retorna nada e recebe como parâmetro a amostra de entrada
        function predict(obj, uk)
            % OBS: essa não é a implementação normal do predict do filtro de kalman o normal seria:
            % obj.xk_minus = obj.A * obj.xk_1_minus + obj.B * uk;
            % Entreanto isso entregar a inclinação relativa (resultado somente da integral)
            % e não os ângulos de Euler.
            % A modificação aqui é para obter os ângulos de Euler, necessário pois será comparado
            % com a estimativa usando acel+mag que também estão em ângulos de Euler

            % Calcula x a priori
            old_ang = obj.xk_1_minus;
            delta_ang = obj.B * uk;
            old_rot = ang2rotZYX(old_ang(1), old_ang(2), old_ang(3));
            delta_rot = ang2rotZYX(delta_ang(1), delta_ang(2), delta_ang(3));
            new_rot = old_rot * delta_rot;
            obj.xk_minus = rot2angZYX(new_rot);
            obj.xk_minus = obj.xk_minus'; % Precisamos dele no formato de vetor para as demais operações

            % Calcula p a priori
            obj.pk_minus = obj.A * obj.pk_1_minus * obj.A' + obj.Q;

            % Atualiza x e k anterior, para o caso de não ser utilizado a função de update
            obj.xk_1_minus = obj.xk_minus;
            obj.pk_1_minus = obj.pk_minus;
        end

        % Realiza a etapa de atualização do algoritmo, salvando no objeto e retornando o valor de x, recebe como parâmetro a amostra de saída
        function xk = update(obj, yk)
            % Atualiza o K
            obj.K = obj.pk_minus * obj.C' / (obj.C * obj.pk_minus * obj.C' + obj.R);

            % atualiza x a posteriori
            diff = subAngle(yk, obj.C * obj.xk_minus);

            obj.xk_minus = sumAngle(obj.xk_minus, obj.K * diff);
            xk = obj.xk_minus;

            % atualiza p a posteriori
            obj.pk_minus = obj.pk_minus - obj.K * obj.C * obj.pk_minus;

            % Atualiza x e k anterior, para calcular a proxima predição
            obj.xk_1_minus = obj.xk_minus;
            obj.pk_1_minus = obj.pk_minus;
        end
    end
end
