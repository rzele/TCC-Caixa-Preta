classdef Car3D < CommonsCar3D
    properties
        data_type % quaternion or euler

        % Chart dependences obj
        some_tilt_chart
    end

    methods
        function obj = Car3D(data_type, some_tilt_chart)
            p_title = sprintf('Rotação 3D usando %s (%s)', data_type, class(some_tilt_chart));
            obj = obj@CommonsCar3D(p_title); % p_title
            obj.data_type = data_type; % quaternion or euler

            if strcmp(obj.data_type, 'euler')
                obj.data = zeros(1, 3);
            elseif strcmp(obj.data_type, 'quaternion')
                obj.data = zeros(1, 4);
            else
                error('Invalid data type. Must be quaternion or euler')
            end

            % Chart dependences obj
            obj.some_tilt_chart = some_tilt_chart;
        end

        function calculate(obj, mpu_new_data, baselines_new_data, n_sample)
            %% Verifica se já calculou essa amostra
            if obj.has_calculated_this_sample(n_sample)
                return
            end

            %% Obtem o valor de outros charts ao qual este é dependente
            obj.some_tilt_chart.calculate(mpu_new_data, baselines_new_data, n_sample);
            new_data = obj.some_tilt_chart.last();
            
            %% Calcula o valor p/ a próxima amostra
            obj.data = new_data;
        end

        function on_update(obj)
            has_error = false;

            if strcmp(obj.data_type, 'euler')
                if length(obj.data) == 3
                    obj.rotateWithEuler(obj.data(1), obj.data(2), obj.data(3));
                else
                    has_error = true;
                    exp_dim = 3;
                end
            elseif strcmp(obj.data_type, 'quaternion')
                if length(obj.data) == 4
                    obj.rotateWithQuaternion(obj.data);
                else
                    has_error = true;
                    exp_dim = 4;
                end
            end

            if has_error
                msg1 = sprintf('Invalid data format for Car3D with quaternion using %s.\n', class(obj.some_tilt_chart));
                msg2 = sprintf('\nThe class %s, must return at method .last() and arr of dimension = %d\n', class(obj.some_tilt_chart), exp_dim);
                error(strcat(msg1, msg2));
            end
        end
    end
end