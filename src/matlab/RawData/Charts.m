classdef Charts < handle
    properties
        % Gráficos possíveis
        Vazio
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
        Madgwick_tilt
        quaternion
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
            obj.Vazio = '';                         % Deixa a celula vazia
            obj.aceleration = Aceleration(cnf.max_size, cnf.window_k);
            obj.gyroscope = Gyroscope(cnf.max_size, cnf.window_k);
            obj.magnetometer = Magnetometer(cnf.max_size, cnf.window_k);
            obj.gyro_relative_tilt = GyroRelativeTilt(cnf.max_size);
            obj.gyro_absolute_tilt = GyroAbsoluteTilt(cnf.max_size);
            obj.acel_mag_tilt = AcelMagTilt(cnf.max_size);
            obj.comp_tilt = CompTilt(cnf.max_size);
            obj.acel_without_g = AcelWithoutG(cnf.max_size);
            obj.velocity = Velocity(cnf.max_size);
            obj.position = Position(cnf.max_size);
            obj.kalman_tilt = KalmanTilt(cnf.max_size, cnf.A, cnf.B, cnf.C, cnf.Q, cnf.R);
            obj.Madgwick_tilt = MadgwickTilt(cnf.max_size, cnf.freq_sample, cnf.beta);
            obj.quaternion = Quaternion(cnf.max_size);
            obj.compass = Compass();
            obj.compass_compensated = CompassCompensated();

            %TODO - criar car3DQuaternios e car3DEuler:
            % assim ao construir o objeto é injetado algum gráfico q retorne eulers ou quaternios.
            % O título do gráfico deve mudar para apontar qual classe foi injetada nele
            obj.car_3d_gdeg = Car3DGdeg();
            obj.car_3d_gtilt = Car3DGtilt();
            obj.car_3d_acelMag = Car3DAcelMag();
            obj.car_3d_comp = Car3DComp();
            obj.car_3d_kalman = Car3DKalman();
            obj.car_3d_madgwick = Car3DMadgwick();
        end
    end
end