classdef Aceleration < TemplateLine
    % Esta classe realizar cálculos de aceleração
    % e plotagem de um gráfico em linha na tela

    properties
        moving_avg
    end

    methods
        function obj = Aceleration()
            obj = obj@TemplateLine(...
                'Aceleração em "g"', ...       % p_title
                'Amostra', ...                 % p_xlabel
                'g', ...                       % p_ylabel
                {'aX', 'aY', 'aZ'}, ...        % s_legend
                {'r', 'g', 'b'});              % sources_color
        end

        function obj = initialize(obj, w_size, f_size)
            obj.w_size = w_size;
            obj.moving_avg = MovingAverage(f_size, 3);
            obj.data = zeros(w_size, 3);
        end

        function calculate(obj, A, a_bias, esc_ac)
            last_data = obj.last();
            new_data = obj.calculate_aceleration(A, a_bias, esc_ac);
            filtered = obj.moving_avg.update(new_data);
            obj.data = [obj.data(2:obj.w_size, :); filtered];
        end
    end
end