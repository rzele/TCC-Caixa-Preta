classdef KalmanTilt < TemplateLine
    % Calcula Rotação usando filtro de Kalman
    % Ref do calculo: https://www.youtube.com/watch?v=urhaoECmCQk
    % e https://www.researchgate.net/publication/261038357_Embedded_Kalman_Filter_for_Inertial_Measurement_Unit_IMU_on_the_ATMega8535

    properties
        kalmanFilterRoll
        kalmanFilterPitch
        kalmanFilterYaw
    end

    methods
        function obj = KalmanTilt(w_size, A,B,C,Q,R)
            obj = obj@TemplateLine(...
                'Filtro de Kalman', ...             % p_title
                'Amostra', ...                      % p_xlabel
                'graus', ...                        % p_ylabel
                {'Roll', 'Pitch', 'Yaw'}, ...       % s_legend
                {'r', 'g', 'b'});                   % sources_color

            obj.w_size = w_size;
            obj.data = zeros(w_size, 3);

            %% Inicializa os filtros de kalman, um para cada eixo,
            % podemos fazer tudo com um filtro só, entretanto os parâmetros ficariam
            % bem grandes e de dificil visualização
            obj.kalmanFilterRoll = Kalman(A,B,C,Q,R);
            obj.kalmanFilterPitch = Kalman(A,B,C,Q,R);
            obj.kalmanFilterYaw = Kalman(A,B,C,Q,R);
        end
        
        function calculate(obj, G, tilt)
            % Calcula a predição p/ cada eixo individualmente
            obj.kalmanFilterRoll.predict(G(1));
            obj.kalmanFilterPitch.predict(G(2));
            obj.kalmanFilterYaw.predict(G(3));

            % Atualiza a predição p/ cada eixo individualmente
            roll_and_bias = obj.kalmanFilterRoll.update(tilt(1));
            pitch_and_bias = obj.kalmanFilterPitch.update(tilt(2));
            yaw_and_bias = obj.kalmanFilterYaw.update(tilt(3));

            % Insere o valor filtrado no eixo a ser plotado
            new_data = [roll_and_bias(1) pitch_and_bias(1) yaw_and_bias(1)];
            obj.data = [obj.data(2:obj.w_size, :); new_data];
        end
    end
end