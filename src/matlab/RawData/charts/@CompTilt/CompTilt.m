classdef CompTilt < TemplateLine
    % Calcula Rotação usando filtro complementar
    % Ref do calculo: https://www.youtube.com/watch?v=whSw42XddsU

    properties
    end

    methods
        function obj = CompTilt()
            obj = obj@TemplateLine(...
                'Filtro complementar', ...          % p_title
                'Amostra', ...                      % p_xlabel
                'graus', ...                        % p_ylabel
                {'Roll', 'Pitch', 'Yaw'}, ...       % s_legend
                {'r', 'g', 'b'});                   % sources_color
        end

        function obj = initialize(obj, fig, w_size)
            obj.w_size = w_size;
            obj.data = zeros(w_size, 3);
        end

        function calculate(obj, gyro_tilt, old_gyro_tilt, tilt, mu)
            new_data = obj.calculate_comp_filter(obj.last(), gyro_tilt, old_gyro_tilt, tilt, mu);
            obj.data = [obj.data(2:obj.w_size, :); new_data];
        end
    end
end