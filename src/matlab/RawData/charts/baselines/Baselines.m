classdef Baselines < CommonsLine
    % Esta classe n�o realiza calculos
    % ela ir� plotar o baselines escolhido via par�metro
    % Esta classe somente pode ser utilizada com os dados de entrada via mockup

    properties
        selected_baseline % pode ser 'position' ou 'tilt'
    end

    methods
        function obj = Baselines(selected_baseline, w_size)
            if ~isOneIn(selected_baseline, {'position', 'tilt'})
                error('Invalid selected_baseline must be "position" or "tilt"');
            end

            unidade = struct('tilt', 'graus', 'position', 'm');
            p_title = sprintf('Baseline (%s)', selected_baseline);
            obj = obj@CommonsLine(...
                p_title, ...                            % p_title
                'Amostra', ...                          % p_xlabel
                unidade.(selected_baseline), ...        % p_ylabel
                {'x', 'y', 'z'}, ...                    % s_legend
                {'r', 'g', 'b'}, ...                    % sources_color
                w_size);

            obj.w_size = w_size;
            obj.data = zeros(w_size, 3);
            obj.selected_baseline = selected_baseline;
        end

        function calculate(obj, mpu_new_data, baselines_new_data, n_sample)
            %% Verifica se j� calculou essa amostra
            if obj.has_calculated_this_sample(n_sample)
                return
            end

            %% Calcula o valor p/ a pr�xima amostra
            if strcmp(obj.selected_baseline, 'tilt')
                % inverte a ordem porque no baseline vem zyx
                new_data = flip(baselines_new_data(5:7));
            else
                new_data = baselines_new_data(2:4);
            end
            
            obj.data = [obj.data(2:obj.w_size, :); new_data];
        end

        function on_delete(obj)
            % Apenas sobrescreve a implementa��o de 'CommonsLine'
            % para n�o exibir nenhum tempo
        end
    end
end