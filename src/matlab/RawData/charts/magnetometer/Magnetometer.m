classdef Magnetometer < CommonsLine
    properties
        moving_avg
        h_offsets
        h_scaleso
    end

    methods
        function obj = Magnetometer(w_size, f_size, h_offsets, h_scaleso)
            obj = obj@CommonsLine(...
                'Magnetrometro em mili Gaus', ...       % p_title
                'Amostra', ...                          % p_xlabel
                'mG', ...                               % p_ylabel
                {'hx', 'hy', 'hz'}, ...                 % s_legend
                {'r', 'g', 'b'}, ...                    % sources_color
                w_size);

            obj.w_size = w_size;
            obj.moving_avg = MovingAverage(f_size, 3);
            obj.data = zeros(w_size, 3);
            obj.h_offsets = h_offsets;
            obj.h_scaleso = h_scaleso;
        end

        function calculate(obj, mpu_new_data, n_sample)
            %% Verifica se já calculou essa amostra
            if obj.has_calculated_this_sample(n_sample)
                return
            end

            %% Calcula o valor p/ a próxima amostra
            H = mpu_new_data(7:9);
            last_data = obj.last();
            new_data = calculate_mag(H, obj.h_offsets, obj.h_scaleso);
            filtered = obj.moving_avg.update(new_data);
            obj.data = [obj.data(2:obj.w_size, :); filtered];
        end
    end
end