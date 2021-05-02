classdef KalmanTilt < CommonsLine
    % Calcula Rotação usando filtro de Kalman
    % Ref do calculo: https://www.youtube.com/watch?v=urhaoECmCQk
    % e https://www.researchgate.net/publication/261038357_Embedded_Kalman_Filter_for_Inertial_Measurement_Unit_IMU_on_the_ATMega8535

    properties
        kalmanFilter
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

            %% Variável de ajuste do filtro de kalman, os valores iniciais de X e P são por padrão 0s
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
            % Portanto, não recomendamos a modificação das
            % matrizes A,B e C, que representam a construção do modelo
            delta = 1/sample_freq;
            A = eye(3);
            B = eye(3) * delta;
            C = eye(3);
            Q = eye(3) * 0.002^2;
            R = eye(3) * 0.03;

            %% Inicializa o filtro de kalman
            obj.kalmanFilter = ModifiedKalmanFilter(A,B,C,Q,R);

            % Chart dependences
            obj.dependencies.gyro = gyro_chart;
            obj.dependencies.some_tilt = some_tilt_chart;
        end
        
        function calculate(obj, mpu_new_data, baselines_new_data, n_sample)
            %% Verifica se já calculou essa amostra
            if obj.has_calculated_this_sample(n_sample)
                return
            end

            %% Obtem o valor de outros charts ao qual este é dependente
            obj.dependencies.gyro.calculate(mpu_new_data, baselines_new_data, n_sample);
            G = obj.dependencies.gyro.last();
            
            obj.dependencies.some_tilt.calculate(mpu_new_data, baselines_new_data, n_sample);
            tilt = obj.dependencies.some_tilt.last();
            
            %% Calcula o valor p/ a próxima amostra
            t = tic();

            % Calcula a predição p/ cada eixo individualmente
            obj.kalmanFilter.predict(G');

            % Atualiza a predição p/ cada eixo individualmente
            new_data = obj.kalmanFilter.update(tilt');

            obj.time = obj.time + toc(t);
            obj.data = [obj.data(2:obj.w_size, :); new_data'];
        end
    end
end