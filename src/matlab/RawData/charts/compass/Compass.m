classdef Compass < CommonsCompass
    % Calcula Compass sem compensa��o 
    % Ref do calculo: https://blog.digilentinc.com/how-to-convert-magnetometer-data-into-compass-heading/
    % e plota em plano polar o Compass sem compensa��o 

    properties
    end

    methods
        function obj = Compass(mag_chart)
            obj = obj@CommonsCompass('Magnetic Heading SEM compensa��o de tilt');      % p_title
            obj.data = 0;

            % Chart dependences
            obj.dependencies.mag = mag_chart;
        end

        function calculate(obj, mpu_new_data, baselines_new_data, n_sample)
            %% Verifica se j� calculou essa amostra
            if obj.has_calculated_this_sample(n_sample)
                return
            end

            %% Obtem o valor de outros charts ao qual este � dependente
            obj.dependencies.mag.calculate(mpu_new_data, baselines_new_data, n_sample);
            H = obj.dependencies.mag.last();
            
            %% Calcula o valor p/ a pr�xima amostra
            new_data = calculate_compass_without_compensation(H);
            obj.data = new_data;
        end
    end
end