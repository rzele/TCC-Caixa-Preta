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
    end
    
    methods
        function obj = Charts(cnf)
            % Instância os plots
            obj.vazio = '';                         % Deixa a celula vazia
            obj.aceleration = Aceleration(cnf.max_size, cnf.window_k, [cnf.ax_bias, cnf.ay_bias, cnf.az_bias], cnf.esc_ac);
            obj.gyroscope = Gyroscope(cnf.max_size, cnf.window_k, [cnf.gx_bias, cnf.gy_bias, cnf.gz_bias], cnf.esc_giro);
            obj.magnetometer = Magnetometer(cnf.max_size, cnf.window_k, [cnf.hx_offset, cnf.hy_offset, cnf.hz_offset], [cnf.hx_scale, cnf.hy_scale, cnf.hz_scale]);
            obj.gyro_relative_tilt = GyroRelativeTilt(cnf.max_size, cnf.freq_sample, obj.gyroscope);
            obj.gyro_absolute_tilt = GyroAbsoluteTilt(cnf.max_size, obj.gyro_relative_tilt);
            obj.acel_mag_tilt = AcelMagTilt(cnf.max_size, obj.aceleration, obj.magnetometer);
            obj.comp_tilt = CompTilt(cnf.max_size, cnf.mu, obj.gyro_relative_tilt, obj.acel_mag_tilt);
            obj.acel_without_g = AcelWithoutG(cnf.max_size, obj.gyro_relative_tilt, obj.aceleration);
            obj.velocity = Velocity(cnf.max_size, cnf.freq_sample, cnf.const_g, obj.acel_without_g);
            obj.position = Position(cnf.max_size, cnf.freq_sample, obj.velocity);
            obj.kalman_tilt = KalmanTilt(cnf.max_size, cnf.A, cnf.B, cnf.C, cnf.Q, cnf.R, obj.gyroscope, obj.acel_mag_tilt);
            obj.madgwick_tilt_quaternion = MadgwickTiltQuaternion(cnf.max_size, cnf.freq_sample, cnf.beta, obj.aceleration, obj.gyroscope, obj.magnetometer);
            obj.madgwick_tilt_euler = MadgwickTiltEuler(cnf.max_size, obj.madgwick_tilt_quaternion);
            
            obj.compass = Compass(obj.magnetometer);
            obj.compass_compensated = CompassCompensated(obj.acel_mag_tilt);
            
            obj.car_3d_gdeg = Car3D('euler', obj.gyro_relative_tilt);
            obj.car_3d_gtilt = Car3D('euler', obj.gyro_absolute_tilt);
            obj.car_3d_acelMag = Car3D('euler', obj.acel_mag_tilt);
            obj.car_3d_comp = Car3D('euler', obj.comp_tilt);
            obj.car_3d_kalman = Car3D('euler', obj.kalman_tilt);
            obj.car_3d_madgwick = Car3D('quaternion', obj.madgwick_tilt_quaternion);
        end
    end
end