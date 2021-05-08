classdef Charts < handle
    properties
        % Gráficos possíveis
        vazio

        aceleration
        gyroscope
        magnetometer
        gyro_relative_tilt
        gyro_absolute_tilt
        acel_mag_tilt
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

        % Variaveis para teste de melhor parâmetro do filtro complementar
        cmp_tst = CustomPlot.empty()
        mdg_tst = CustomPlot.empty()
    end
    
    methods
        function obj = Charts(cnf)
            %% Cria gráficos de linha
            obj.vazio = '';                         % Deixa a celula vazia
            obj.aceleration = Aceleration(cnf.filter_type, cnf.max_size, cnf.window_k, [cnf.cxp.ax_offset, cnf.cxp.ay_offset, cnf.cxp.az_offset], cnf.cxp.aesc_op);
            obj.gyroscope = Gyroscope(cnf.filter_type, cnf.max_size, cnf.window_k, [cnf.cxp.gx_offset, cnf.cxp.gy_offset, cnf.cxp.gz_offset], cnf.cxp.gesc_op);
            obj.magnetometer = Magnetometer(cnf.filter_type, cnf.max_size, cnf.window_k, [cnf.cxp.hx_offset, cnf.cxp.hy_offset, cnf.cxp.hz_offset], [cnf.cxp.hx_escala, cnf.cxp.hy_escala, cnf.cxp.hz_escala]);
            obj.gyro_relative_tilt = GyroRelativeTilt(cnf.max_size, cnf.cxp.fammost, obj.gyroscope);
            obj.gyro_absolute_tilt = GyroAbsoluteTilt(cnf.max_size, obj.gyro_relative_tilt);
            obj.acel_mag_tilt = AcelMagTilt(cnf.max_size, obj.aceleration, obj.magnetometer);
            obj.comp_tilt = CompTilt(cnf.max_size, cnf.mu, obj.gyro_absolute_tilt, obj.acel_mag_tilt);
            obj.acel_without_g = AcelWithoutG(cnf.max_size, obj.gyro_relative_tilt, obj.aceleration);
            obj.velocity = Velocity(cnf.max_size, cnf.cxp.fammost, cnf.const_g, obj.acel_without_g);
            obj.position = Position(cnf.max_size, cnf.cxp.fammost, obj.velocity);
            obj.kalman_tilt = KalmanTilt(cnf.max_size, cnf.cxp.fammost, obj.gyroscope, obj.acel_mag_tilt);
            obj.madgwick_tilt_quaternion = MadgwickTiltQuaternion(cnf.max_size, cnf.cxp.fammost, cnf.beta, obj.aceleration, obj.gyroscope, obj.magnetometer);
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
            obj.baseline_tilt = Baselines('tilt', cnf.max_size);
            obj.baseline_position = Baselines('position', cnf.max_size);
            obj.car_3d_baseline = Car3D('euler', obj.baseline_tilt);

            %% Cria gráficos de comparação de métodos de estimativa de tilt
            % Nas configurações de layout você pode utilizar tanto a struct aqui retornada
            % c.compare_tilts.yaw (.pitch ou .roll)
            % ou pode utilizar os apelidos abaixo: c.compare_rolls, c.compare_pitchs, c.compare_yaws
            % Você pode alterar os métodos que serão comparados no array abaixo
            obj.compare_tilts = CompareTilts.factory_row_pitch_yaw(cnf.max_size, obj.baseline_tilt,...
                [obj.gyro_absolute_tilt, obj.acel_mag_tilt, obj.comp_tilt]);
            % Apelidos
            obj.compare_rolls = obj.compare_tilts.roll;
            obj.compare_pitchs = obj.compare_tilts.pitch;
            obj.compare_yaws = obj.compare_tilts.yaw;
            obj.compare_errors = obj.compare_tilts.errors;

            % Cria gráficos para comparar parâmetros dos filtros
            % Irá sobrescrever o compare_tilts criado acima
            % obj.set_comp_tilt_test(cnf);
            obj.set_madgwick_tilt_test(cnf);
        end

        function set_comp_tilt_test(obj, cnf)
            count = 1;
            for mu=[0:0.0001:0.005]
                obj.cmp_tst(count) = CompTilt(cnf.max_size, mu, obj.gyro_absolute_tilt, obj.acel_mag_tilt);
                count = count+1;
            end

            obj.compare_tilts = CompareTilts.factory_row_pitch_yaw(cnf.max_size, obj.baseline_tilt, obj.cmp_tst);
        end

        function set_madgwick_tilt_test(obj, cnf)
            count = 1;
            
            for beta=[0:0.001:0.05]
                madg_quat = MadgwickTiltQuaternion(cnf.max_size, cnf.cxp.fammost, beta, obj.aceleration, obj.gyroscope, obj.magnetometer);
                obj.mdg_tst(count) = MadgwickTiltEuler(cnf.max_size, madg_quat);
                count = count+1;
            end

            obj.compare_tilts = CompareTilts.factory_row_pitch_yaw(cnf.max_size, obj.baseline_tilt, obj.mdg_tst);
        end
    end
end