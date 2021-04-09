classdef MadgwickTiltEuler < CommonsLine
    properties
    end

    methods
        function obj = MadgwickTiltEuler(w_size, madgwick_q_chart)
            obj = obj@CommonsLine(...
                'Filtro de Madgwick', ...               % p_title
                'Amostra', ...                          % p_xlabel
                'graus', ...                            % p_ylabel
                {'Roll', 'Pitch', 'Yaw'}, ...           % s_legend
                {'r', 'g', 'b'}, ...                    % sources_color
                w_size);

            obj.w_size = w_size;
            obj.data = zeros(w_size, 3);

            % Chart dependences
            obj.dependencies.madgwick_q = madgwick_q_chart;
        end

        function calculate(obj, mpu_new_data, baselines_new_data, n_sample)
            %% Verifica se já calculou essa amostra
            if obj.has_calculated_this_sample(n_sample)
                return
            end

            %% Obtem o valor de outros charts ao qual este é dependente
            obj.dependencies.madgwick_q.calculate(mpu_new_data, baselines_new_data, n_sample);
            q = obj.dependencies.madgwick_q.last();
            
            %% Calcula o valor p/ a próxima amostra
            t = tic();

            euler = quatern2euler(quaternConj(q)) * (180/pi);
          
            obj.time = obj.time + toc(t);
            obj.data = [obj.data(2:obj.w_size, :); euler];
        end
    end
end