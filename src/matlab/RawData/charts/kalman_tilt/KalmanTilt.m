classdef KalmanTilt < CommonsLine
    % Calcula Rota��o usando filtro de Kalman
    % Ref do calculo: https://www.youtube.com/watch?v=urhaoECmCQk
    % e https://www.researchgate.net/publication/261038357_Embedded_Kalman_Filter_for_Inertial_Measurement_Unit_IMU_on_the_ATMega8535

    properties
        kalmanFilterRoll
        kalmanFilterPitch
        kalmanFilterYaw
    end

    methods
        function obj = KalmanTilt(w_size, sample_freq, gyro_chart, some_tilt_chart)
            p_title = sprintf('Filtro de Kalman (Gyro + %s)', class(some_tilt_chart));
            obj = obj@CommonsLine(...
                p_title, ...             % p_title
                'Amostra', ...                      % p_xlabel
                'graus', ...                        % p_ylabel
                {'Roll', 'Pitch', 'Yaw'}, ...       % s_legend
                {'r', 'g', 'b'}, ...                % sources_color
                w_size);

            obj.w_size = w_size;
            obj.data = zeros(w_size, 3);

            %% Vari�vel de ajuste do filtro de kalman, os valores iniciais de X e P s�o por padr�o 0s
            % Nosso modelo countem:
            % - 2 entrada (uk = delta Giro/s e yk = Tilt do acelerometro)
            % - 1 estados (x = Tilt usando Giro)
            % portanto nosso modelo fica:
            %
            % x[k] = A*x[k-1] + B*u[k]
            % x[k] = x[k-1] + 1/freq * u[k]
            %
            % y[k] = C*x[k]
            % y[k] = x[k]
            % 
            % OBS: O filtro de kalman utilizado foi modificado para manter os intervalos
            % entre -180,180. Para entender melhor veja o arquivo 'ModifiedKalmanFilter.m'
            % Portanto, n�o recomendamos a modifica��o das
            % matrizes A,B e C, que representam a constru��o do modelo
            A = 1;
            B = 1/sample_freq;
            C = 1;
            Q = 0.002^2;
            R = 0.03;

            %% Inicializa os filtros de kalman, um para cada eixo,
            % podemos fazer tudo com um filtro s�, entretanto os par�metros ficariam
            % bem grandes e de dificil visualiza��o
            obj.kalmanFilterRoll = ModifiedKalmanFilter(A,B,C,Q,R);
            obj.kalmanFilterPitch = ModifiedKalmanFilter(A,B,C,Q,R);
            obj.kalmanFilterYaw = ModifiedKalmanFilter(A,B,C,Q,R);

            % Chart dependences
            obj.dependencies.gyro = gyro_chart;
            obj.dependencies.some_tilt = some_tilt_chart;
        end
        
        function calculate(obj, mpu_new_data, baselines_new_data, n_sample)
            %% Verifica se j� calculou essa amostra
            if obj.has_calculated_this_sample(n_sample)
                return
            end

            %% Obtem o valor de outros charts ao qual este � dependente
            obj.dependencies.gyro.calculate(mpu_new_data, baselines_new_data, n_sample);
            G = obj.dependencies.gyro.last();
            
            obj.dependencies.some_tilt.calculate(mpu_new_data, baselines_new_data, n_sample);
            tilt = obj.dependencies.some_tilt.last();
            
            %% Calcula o valor p/ a pr�xima amostra
            t = tic();

            % Calcula a predi��o p/ cada eixo individualmente
            obj.kalmanFilterRoll.predict(G(1));
            obj.kalmanFilterPitch.predict(G(2));
            obj.kalmanFilterYaw.predict(G(3));

            % Atualiza a predi��o p/ cada eixo individualmente
            roll_and_bias = obj.kalmanFilterRoll.update(tilt(1));
            pitch_and_bias = obj.kalmanFilterPitch.update(tilt(2));
            yaw_and_bias = obj.kalmanFilterYaw.update(tilt(3));

            % Insere o valor filtrado no eixo a ser plotado
            new_data = [roll_and_bias(1) pitch_and_bias(1) yaw_and_bias(1)];
            
            obj.time = obj.time + toc(t);
            obj.data = [obj.data(2:obj.w_size, :); new_data];
        end
    end
end