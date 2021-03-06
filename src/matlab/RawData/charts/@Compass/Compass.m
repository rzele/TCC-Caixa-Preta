classdef Compass < CommonsCompass
    % Calcula Compass sem compensação 
    % Ref do calculo: https://blog.digilentinc.com/how-to-convert-magnetometer-data-into-compass-heading/
    % e plota em plano polar o Compass sem compensação 

    properties
        % Chart dependences obj
        mag_chart
    end

    methods
        function obj = Compass(mag_chart)
            obj = obj@CommonsCompass('Magnetic Heading SEM compensação de tilt');      % p_title
            obj.data = 0;

            % Chart dependences
            obj.mag_chart = mag_chart;
        end

        function calculate(obj, mpu_new_data, n_sample)
            %% Verifica se já calculou essa amostra
            if obj.has_calculated_this_sample(n_sample)
                return
            end

            %% Obtem o valor de outros charts ao qual este é dependente
            obj.mag_chart.calculate(mpu_new_data, n_sample);
            H = obj.mag_chart.last();
            
            %% Calcula o valor p/ a próxima amostra
            new_data = obj.compass_without_compensation(H);
            obj.data = new_data;
        end
    end
end