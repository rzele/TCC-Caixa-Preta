classdef Magnetometer < CommonsLine
    properties
        filter_type % pode ser 'media' ou 'mediana'
        
        filter
        h_offsets
        h_scaleso
    end

    methods
        function obj = Magnetometer(filter_type, w_size, f_size, h_offsets, h_scaleso)
            p_title = sprintf('Magnetrometro em mili Gaus(%s=%d)', filter_type, f_size);
            obj = obj@CommonsLine(...
                p_title, ...                            % p_title
                'Amostra', ...                          % p_xlabel
                'mG', ...                               % p_ylabel
                {'hx', 'hy', 'hz'}, ...                 % s_legend
                {'r', 'g', 'b'}, ...                    % sources_color
                w_size);

            obj.filter_type = filter_type;
            obj.w_size = w_size;
            obj.data = zeros(w_size, 3);
            obj.h_offsets = h_offsets;
            obj.h_scaleso = h_scaleso;

            if strcmp(filter_type, 'media')
                obj.filter = MovingAverage(f_size, 3);
            elseif strcmp(filter_type, 'mediana')
                obj.filter = MovingMedian(f_size, 3);
            else
                error('Invalid filter_type must be media or mediana');
            end
        end

        function calculate(obj, mpu_new_data, baselines_new_data, n_sample)
            %% Verifica se já calculou essa amostra
            if obj.has_calculated_this_sample(n_sample)
                return
            end

            %% Calcula o valor p/ a próxima amostra
            t = tic();
            
            H = mpu_new_data(7:9);
            last_data = obj.last();
            new_data = calculate_mag(H, obj.h_offsets, obj.h_scaleso);
            filtered = obj.filter.calculate(new_data);
            
            obj.time = obj.time + toc(t);
            obj.data = [obj.data(2:obj.w_size, :); filtered];
        end
    end
end