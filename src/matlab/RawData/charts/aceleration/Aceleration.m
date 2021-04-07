classdef Aceleration < CommonsLine
    % Esta classe realizar cálculos de aceleração
    % e plotagem de um gráfico em linha na tela

    properties
        filter_type % pode ser 'media' ou 'mediana'

        filter
        a_bias
        esc_ac
    end

    methods
        function obj = Aceleration(filter_type, w_size, f_size, a_bias, esc_ac)
            p_title = sprintf('Aceleração em "g"(%s=%d)', filter_type, f_size);
            obj = obj@CommonsLine(...
                p_title, ...                   % p_title
                'Amostra', ...                 % p_xlabel
                'g', ...                       % p_ylabel
                {'aX', 'aY', 'aZ'}, ...        % s_legend
                {'r', 'g', 'b'}, ...           % sources_color
                w_size);

            obj.filter_type = filter_type;
            obj.w_size = w_size;
            obj.data = zeros(w_size, 3);
            obj.a_bias = a_bias;
            obj.esc_ac = esc_ac;

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
            A = mpu_new_data(1:3);
            last_data = obj.last();
            new_data = calculate_aceleration(A, obj.a_bias, obj.esc_ac);
            filtered = obj.filter.calculate(new_data);
            obj.data = [obj.data(2:obj.w_size, :); filtered];
        end
    end
end