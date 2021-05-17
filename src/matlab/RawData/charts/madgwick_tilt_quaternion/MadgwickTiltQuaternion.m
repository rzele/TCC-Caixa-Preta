classdef MadgwickTiltQuaternion < CommonsLine
    % Calcula Rotação usando filtro de madgwick
    % Ref do calculo: https://nitinjsanket.github.io/tutorials/attitudeest/madgwick
    % e https://x-io.co.uk/open-source-imu-and-ahrs-algorithms/
    
    properties
        madgwickFilter
        acel_mag_tilt
        beta
    end

    methods
        function obj = MadgwickTiltQuaternion(w_size, freq_sample, beta, acel_chart, gyro_chart, mag_chart, acel_mag_tilt_chart)
            obj = obj@CommonsLine(...
                'Quaterions do filtro de Madgwick', ...       % p_title
                'Amostra', ...                                % p_xlabel
                'val', ...                                    % p_ylabel
                {'q1', 'q2', 'q3', 'q4'}, ...                 % s_legend
                {'r', 'g', 'b', 'y'}, ...                     % sources_color
                w_size);

            obj.w_size = w_size;
            obj.data = zeros(w_size, 4);
            obj.beta = beta;

            % Inicializa o filtro de madgwick
            obj.madgwickFilter = MadgwickAHRS('SamplePeriod', 1/freq_sample, 'Beta', beta);

            % Chart dependences
            obj.dependencies.acel = acel_chart;
            obj.dependencies.gyro = gyro_chart;
            obj.dependencies.mag = mag_chart;

            % Não é exatamente uma dependência
            % Essa estimativa é utilizada somente para definir a inclinação inicial do corpo
            obj.acel_mag_tilt = acel_mag_tilt_chart;
        end

        function calculate(obj, mpu_new_data, baselines_new_data, n_sample)
            %% Verifica se já calculou essa amostra
            if obj.has_calculated_this_sample(n_sample)
                return
            end

            %% Obtem o valor de outros charts ao qual este é dependente
            obj.dependencies.gyro.calculate(mpu_new_data, baselines_new_data, n_sample);
            G = obj.dependencies.gyro.last();

            obj.dependencies.acel.calculate(mpu_new_data, baselines_new_data, n_sample);
            A = obj.dependencies.acel.last();

            obj.dependencies.mag.calculate(mpu_new_data, baselines_new_data, n_sample);
            H = obj.dependencies.mag.last();
            
            %% Calcula o valor p/ a próxima amostra
            t = tic();

            % Para a amostra inicial define como estimativa aquela gerada pelo
            % Acelerômetro + magnetômetro
            if (n_sample < 10)
                obj.acel_mag_tilt.calculate(mpu_new_data, baselines_new_data, n_sample);

                initial_tilt = obj.acel_mag_tilt.last();
                initial_rot = ang2rotZYX(initial_tilt(1),initial_tilt(2),initial_tilt(3));
                q_conj = rotMat2quatern(initial_rot);
                initial_q = quaternConj(q_conj);

                obj.madgwickFilter.Quaternion = initial_q;
            end

            obj.madgwickFilter.Update(G*pi/180, A, H);
            q = obj.madgwickFilter.Quaternion;

            obj.time = obj.time + toc(t);
            obj.data = [obj.data(2:obj.w_size, :); q];
        end
    end
end