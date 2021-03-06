classdef AcelMagTilt < CommonsLine
    % Calcula Yaw, Pitch e Roll absolutos(em relação a Posição inicial do corpo) p/ a nova amostra usando Aceleração e Magnetômetro
    % Usando a Aceleração, usamos o vetor de gravidade que deve sempre
    % estar presente p/ determinar a Posição do corpo
    % assim temos que p/ a orderm de Rotação Z,Y,X (note que na referencia ele faz o calculo com XYZ, isso se deve por conta do lado da equação ao qual a matriz aparece)
    % Ref do calculo: https://www.nxp.com/docs/en/application-note/AN3461.pdf

    properties
        % Chart dependences obj
        acel_chart
        mag_chart
    end

    methods
        function obj = AcelMagTilt(w_size, acel_chart, mag_chart)
            obj = obj@CommonsLine(...
                'Giro em graus(absoluto) usando acel + mag', ...     % p_title
                'Amostra', ...                                       % p_xlabel
                'graus', ...                                         % p_ylabel
                {'aRoll', 'aPitch', 'mYaw'}, ...                     % s_legend
                {'r', 'g', 'b'});                                    % sources_color
            
            obj.w_size = w_size;
            obj.data = zeros(w_size, 3);

            % Chart dependences
            obj.acel_chart = acel_chart;
            obj.mag_chart = mag_chart;
        end
        
        function calculate(obj, mpu_new_data, n_sample)
            %% Verifica se já calculou essa amostra
            if obj.has_calculated_this_sample(n_sample)
                return
            end

            %% Obtem o valor de outros charts ao qual este é dependente
            obj.acel_chart.calculate(mpu_new_data, n_sample);
            A = obj.acel_chart.last();

            obj.mag_chart.calculate(mpu_new_data, n_sample);
            H = obj.mag_chart.last();

            %% Calcula o valor p/ a próxima amostra
            new_data = calculate_acel_mag_tilt(A, H);
            obj.data = [obj.data(2:obj.w_size, :); new_data];
        end
    end
end