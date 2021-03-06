classdef Quaternion < TemplateLine
    properties
    end

    methods
        function obj = Quaternion(w_size)
            obj = obj@TemplateLine(...
                'Quaterions do filtro de Madgwick', ...       % p_title
                'Amostra', ...                                % p_xlabel
                'val', ...                                    % p_ylabel
                {'q1', 'q2', 'q3', 'q4'}, ...                 % s_legend
                {'r', 'g', 'b', 'y'});                        % sources_color

            obj.w_size = w_size;
            obj.data = zeros(w_size, 4);
        end

        function calculate(obj, q)
            obj.data = [obj.data(2:obj.w_size, :); q];
        end
    end
end