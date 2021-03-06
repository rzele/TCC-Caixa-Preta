classdef GyroRelativeTilt < TemplateLine
    % Calcula Yaw, Pitch e Roll realtivos(em relação a ultima Rotação do corpo) p/ a nova amostra usando giro
    % Usando o giro, fazemos a integral discreta (área do trapézio aculmulado)
    % P/ o novo dado isso significa, ultimo valor + novo trapézio (entre ultimo dado e o novo)
    % É considerado nesse calculo que, as amostragens estão espaçadas de 1
    % periodo da amostragem, ent o trapézio é igual a 1/freq * ((n-1 + n)/2)

    properties
    end

    methods
        function obj = GyroRelativeTilt(w_size)
            obj = obj@TemplateLine(...
                'Giro em graus(relativo)', ...      % p_title
                'Amostra', ...                      % p_xlabel
                'graus', ...                        % p_ylabel
                {'Roll', 'Pitch', 'Yaw'}, ...       % s_legend
                {'r', 'g', 'b'});                   % sources_color

            obj.w_size = w_size;
            obj.data = zeros(w_size, 3);
        end

        function calculate(obj, G, old_G, freq_sample)
            new_data = obj.calculate_gyro_relative_tilt(obj.last(), G, old_G, freq_sample);
            obj.data = [obj.data(2:obj.w_size, :); new_data];
        end
    end
end