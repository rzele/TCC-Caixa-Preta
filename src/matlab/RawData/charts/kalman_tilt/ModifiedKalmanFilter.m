classdef ModifiedKalmanFilter < handle
    %KALMAN Implementa e modifica o filtro de kalman para estimativa de inclina��o
    %   OBS: Esta implemanta��o modifica o filtro de kalman para manter a
    %   subtra��o (medida - estimativa) dentro do intervalo -180,180.
    %   Esta mofifica��o � necess�rio conforme exemplificado mais abaixo!
    %   Este filtro somente funciona para o modelo criado, ou seja,
    %   os valores de A,B,C definidos.
    %   Para utilizar com outro modelo, crie uma c�pia e remova os conjuntos de if/else mais abaixo.
    %
    %   Formato padr�o de modelagem:
    %   x[k] = A*x[k-1] + B*u[k] + w[k]
    %   y[k] = C*x[k] + v[k]
    %   E[wwt] = Q
    %   E[vvt] = R
    %
    % OBS: As opera��es sumAngle e subAngle, n�o s�o da implementa��o normal do filtro de kalman.
    % Ela � utilizada pois precisamos que os angulos estejam entre -180,180
    % para n�o dar problema na subtra��o no momento de atualiza��o. 
    % E.g.:
    %   Se nossa ultima estimativa foi -180 e a nova medida for 180, a diferen�a fica
    %   -180 - (-180) = 360
    %   Atualizar a estimativa atual de k*360 e diferente de k*0
    % Ao utilizar ao realiar soma e subtra��o de angulos utilizado a fun��o, garantimos o intervalo desejado.
    % Por isso essa implementa��o � espec�fica para o modelo utilizado do filtro de kalman.

    properties
        xk_minus                % x estimado p/ o instante k
        pk_minus                % p estimado p/ o instante k
        xk_1_minus              % x estimado p/ o instante k-1
        pk_1_minus              % p estimado p/ o instante k-1

        K                       % Matriz de ajuste

        Q                       % Erro do valor de entrada
        R                       % Erro do valor de sa�da do Model

        A                       % (Matriz din�mica)
        B                       % (Matriz de entrada)
        C                       % (Matriz de sa�da)
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

        % Realiza a etapa de predi��o do algoritmo, salvando no objeto os valores preditos, n�o retorna nada e recebe como par�metro a amostra de entrada
        function predict(obj, uk)
            % Calcula x a priori
            obj.xk_minus = obj.A * obj.xk_1_minus + obj.B * uk;

            % Calcula p a priori
            obj.pk_minus = obj.A * obj.pk_1_minus * obj.A' + obj.Q;

            % Atualiza x e k anterior, para o caso de n�o ser utilizado a fun��o de update
            obj.xk_1_minus = obj.xk_minus;
            obj.pk_1_minus = obj.pk_minus;
        end

        % Realiza a etapa de atualiza��o do algoritmo, salvando no objeto e retornando o valor de x, recebe como par�metro a amostra de sa�da
        function xk = update(obj, yk)
            % Atualiza o K
            obj.K = obj.pk_minus * obj.C' / (obj.C * obj.pk_minus * obj.C' + obj.R);

            % atualiza x a posteriori
            diff = yk - obj.C * obj.xk_minus;

            obj.xk_minus = obj.xk_minus + obj.K * diff;
            xk = obj.xk_minus;

            % atualiza p a posteriori
            obj.pk_minus = obj.pk_minus - obj.K * obj.C * obj.pk_minus;

            % Atualiza x e k anterior, para calcular a proxima predi��o
            obj.xk_1_minus = obj.xk_minus;
            obj.pk_1_minus = obj.pk_minus;
        end
    end
end
