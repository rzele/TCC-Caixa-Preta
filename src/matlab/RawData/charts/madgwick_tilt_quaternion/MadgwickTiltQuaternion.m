classdef MadgwickTiltQuaternion < CommonsLine
    % Calcula Rotação usando filtro de madgwick
    % Ref do calculo: https://nitinjsanket.github.io/tutorials/attitudeest/madgwick
    % e https://x-io.co.uk/open-source-imu-and-ahrs-algorithms/
    
    properties
        madgwickFilter

        % Chart dependences obj
        acel_chart
        gyro_chart
        mag_chart
    end

    methods
        function obj = MadgwickTiltQuaternion(w_size, freq_sample, beta, acel_chart, gyro_chart, mag_chart)
            obj = obj@CommonsLine(...
                'Quaterions do filtro de Madgwick', ...       % p_title
                'Amostra', ...                                % p_xlabel
                'val', ...                                    % p_ylabel
                {'q1', 'q2', 'q3', 'q4'}, ...                 % s_legend
                {'r', 'g', 'b', 'y'}, ...                     % sources_color
                w_size);

            obj.w_size = w_size;
            obj.data = zeros(w_size, 4);

            % Inicializa o filtro de madgwick
            obj.madgwickFilter = MadgwickAHRS('SamplePeriod', 1/freq_sample, 'Beta', beta);

            % Chart dependences
            obj.acel_chart = acel_chart;
            obj.gyro_chart = gyro_chart;
            obj.mag_chart = mag_chart;
        end

        function calculate(obj, mpu_new_data, baselines_new_data, n_sample)
            %% Verifica se já calculou essa amostra
            if obj.has_calculated_this_sample(n_sample)
                return
            end

            %% Obtem o valor de outros charts ao qual este é dependente
            obj.gyro_chart.calculate(mpu_new_data, baselines_new_data, n_sample);
            G = obj.gyro_chart.last();

            obj.acel_chart.calculate(mpu_new_data, baselines_new_data, n_sample);
            A = obj.acel_chart.last();

            obj.mag_chart.calculate(mpu_new_data, baselines_new_data, n_sample);
            H = obj.mag_chart.last();
            
            %% Calcula o valor p/ a próxima amostra
            obj.madgwickFilter.Update(G*pi/180, A, H);
            q = obj.madgwickFilter.Quaternion;
            obj.data = [obj.data(2:obj.w_size, :); q];
        end
    end
end