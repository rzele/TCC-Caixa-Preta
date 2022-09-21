classdef Charts < handle
    properties
        % Gráficos possíveis
        vazio

        % Dado filtrado
        aceleration
        gyroscope
        magnetometer
        acel_mag_tilt
        gyro_relative_tilt
        gyro_absolute_tilt
        comp_tilt
        acel_without_g
        velocity
        position
        kalman_tilt
        madgwick_tilt_quaternion
        madgwick_tilt_euler

        compass
        compass_compensated

        car_3d_gdeg
        car_3d_gtilt
        car_3d_acelMag
        car_3d_comp
        car_3d_kalman
        car_3d_madgwick

        compare_tilts
        compare_rolls
        compare_pitchs
        compare_yaws
        compare_errors

        baseline_tilt
        baseline_position
        car_3d_baseline
    end

    methods
        function obj = Charts(cnf)
            %% Cria gráficos de linha
            obj.vazio = '';                         % Deixa a celula vazia

            % Dado filtrado com janela móvel (cnf.window_k) usado para exibir os dados visualmente
            obj.aceleration = Aceleration(cnf.filter_type, cnf.max_size, cnf.window_k, [cnf.cxp.ax_offset, cnf.cxp.ay_offset, cnf.cxp.az_offset], cnf.cxp.aesc_op);
            obj.gyroscope = Gyroscope(cnf.filter_type, cnf.max_size, cnf.window_k, [cnf.cxp.gx_offset, cnf.cxp.gy_offset, cnf.cxp.gz_offset], cnf.cxp.gesc_op);
            obj.magnetometer = Magnetometer(cnf.filter_type, cnf.max_size, cnf.window_k, [cnf.cxp.hx_offset, cnf.cxp.hy_offset, cnf.cxp.hz_offset], [cnf.cxp.hx_escala, cnf.cxp.hy_escala, cnf.cxp.hz_escala]);
            obj.acel_mag_tilt = AcelMagTilt(cnf.max_size, obj.aceleration, obj.magnetometer);

            % Dado não filtrado com janela móvel utilizado somente como input de outros métodos
            % A falta de janela móvel melhora acurácia, e não para plotar
            aceleration_w_k = Aceleration(cnf.filter_type, cnf.max_size, 1, [cnf.cxp.ax_offset, cnf.cxp.ay_offset, cnf.cxp.az_offset], cnf.cxp.aesc_op);
            gyroscope_w_k = Gyroscope(cnf.filter_type, cnf.max_size, 1, [cnf.cxp.gx_offset, cnf.cxp.gy_offset, cnf.cxp.gz_offset], cnf.cxp.gesc_op);
            magnetometer_w_k = Magnetometer(cnf.filter_type, cnf.max_size, 1, [cnf.cxp.hx_offset, cnf.cxp.hy_offset, cnf.cxp.hz_offset], [cnf.cxp.hx_escala, cnf.cxp.hy_escala, cnf.cxp.hz_escala]);
            acel_mag_tilt_w_k = AcelMagTilt(cnf.max_size, aceleration_w_k, magnetometer_w_k);

            %% ====================== Cálculos  finais ====================== %%

            % passo intermediário para gerar o ângulo absoluto
            obj.gyro_relative_tilt = GyroRelativeTilt(cnf.max_size, cnf.cxp.fammost, gyroscope_w_k);

            % ângulos gerados
            obj.gyro_absolute_tilt = GyroAbsoluteTilt(cnf.max_size, obj.gyro_relative_tilt); % só integração
            obj.comp_tilt = CompTilt(cnf.max_size, cnf.mu, obj.gyro_absolute_tilt, acel_mag_tilt_w_k); % filtrado com magnetômetro para melhor resultado

            % passo intermediário para calcular velocidade e posição
            obj.acel_without_g = AcelWithoutG(cnf.max_size, obj.gyro_relative_tilt, aceleration_w_k);

            % Dados bem meh, falta processamento
            obj.velocity = Velocity(cnf.max_size, cnf.cxp.fammost, cnf.const_g, obj.acel_without_g);
            obj.position = Position(cnf.max_size, cnf.cxp.fammost, obj.velocity);

            % ângulos gerados finais
            obj.kalman_tilt = KalmanTilt(cnf.max_size, cnf.cxp.fammost, gyroscope_w_k, acel_mag_tilt_w_k); % Euler
            obj.madgwick_tilt_quaternion = MadgwickTiltQuaternion(cnf.max_size, cnf.cxp.fammost, cnf.beta, aceleration_w_k, obj.gyroscope, magnetometer_w_k, acel_mag_tilt_w_k);
            obj.madgwick_tilt_euler = MadgwickTiltEuler(cnf.max_size, obj.madgwick_tilt_quaternion);

            %% Cria gráficos de bússola
            obj.compass = Compass(obj.magnetometer);
            obj.compass_compensated = CompassCompensated(obj.acel_mag_tilt);

            %% Cria gráficos de carrinho
            obj.car_3d_gdeg = Car3D('euler', obj.gyro_relative_tilt);
            obj.car_3d_gtilt = Car3D('euler', obj.gyro_absolute_tilt);
            obj.car_3d_acelMag = Car3D('euler', obj.acel_mag_tilt);
            obj.car_3d_comp = Car3D('euler', obj.comp_tilt);
            obj.car_3d_kalman = Car3D('euler', obj.kalman_tilt);
            obj.car_3d_madgwick = Car3D('quaternion', obj.madgwick_tilt_quaternion);

            %% Cria gráficos de baseline (somente p/ leitura usando mockup)
            % valor esperado caso fosse perfeito
            obj.baseline_tilt = Baselines('tilt', cnf.max_size);
            obj.baseline_position = Baselines('position', cnf.max_size);
            obj.car_3d_baseline = Car3D('euler', obj.baseline_tilt);

            %% Cria gráficos de comparação de métodos de estimativa de tilt
            % Nas configurações de layout você pode utilizar tanto a struct aqui retornada
            % c.compare_tilts.yaw (.pitch ou .roll)
            % ou pode utilizar os apelidos abaixo: c.compare_rolls, c.compare_pitchs, c.compare_yaws
            % Você pode alterar os métodos que serão comparados no array abaixo
            obj.compare_tilts = CompareTilts.factory_row_pitch_yaw(cnf.max_size, obj.baseline_tilt,...
                [obj.acel_mag_tilt, obj.gyro_absolute_tilt, obj.comp_tilt, obj.kalman_tilt, obj.madgwick_tilt_euler]);
            % Apelidos
            obj.compare_rolls = obj.compare_tilts.roll;
            obj.compare_pitchs = obj.compare_tilts.pitch;
            obj.compare_yaws = obj.compare_tilts.yaw;
            obj.compare_errors = obj.compare_tilts.errors;
        end
    end
end
