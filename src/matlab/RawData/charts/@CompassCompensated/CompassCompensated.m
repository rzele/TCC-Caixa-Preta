classdef CompassCompensated < CommonsCompass
    % Usa o valor calculado do compass com compensação já calculado acima
    % Ref do calculo: https://www.mikrocontroller.net/attachment/292888/AN4248.pdf

    properties
        % Chart dependences obj
        acel_mag_tilt_chart
    end

    methods
        function obj = CompassCompensated(acel_mag_tilt_chart)
            obj = obj@CommonsCompass('Magnetic Heading COM compensação de tilt');      % p_title
            obj.data = 0;

            % Chart dependences
            obj.acel_mag_tilt_chart = acel_mag_tilt_chart;
        end

        function calculate(obj, mpu_new_data, n_sample)
            %% Verifica se já calculou essa amostra
            if obj.has_calculated_this_sample(n_sample)
                return
            end

            %% Obtem o valor de outros charts ao qual este é dependente
            obj.acel_mag_tilt_chart.calculate(mpu_new_data, n_sample);
            acel_mag_last = obj.acel_mag_tilt_chart.last();
            yaw = acel_mag_last(3);
            
            %% Calcula o valor p/ a próxima amostra
            obj.data = yaw;
        end
    end
end